    #include "Game.h"
    #include "CombatSystem.h"
    #include "MapLoader.h"
    #include <iostream>

Game::Game(const std::string& name1,const std::string& name2)
{
    players.push_back(std::make_unique<Player>(1));
    players.push_back(std::make_unique<Player>(2));

    players[0]->name = name1;
    players[1]->name = name2;

    initializeBoard();
}

    void Game::initializePlayers() {
        players.clear();
        players.push_back(std::make_unique<Player>(1));
        players.push_back(std::make_unique<Player>(2));
    }

    void Game::initializeBoard() {
        // Board loaded elsewhere
    }

    void Game::startGame() {
        if (gameStarted) return;

        gameStarted = true;
        gameOver = false;
        turnCount = 1;
        currentPlayerIndex = 0;

        std::cout << "=== GAME STARTED ===\n";
        std::cout << "Player 1 vs Player 2\n";

        drawCard();
        displayTurnInfo();
        displayGameState();
    }

    bool Game::drawCard() {
        if (gameOver) return false;

        Player* current = getCurrentPlayer();
        currentCard = current->drawCard();

        if (!currentCard) {
            std::cout << current->getName() << " has no cards to draw!\n";
            return false;
        }

        std::cout << current->getName() << " draws: ";
        currentCard->display();
        return true;
    }

    void Game::nextTurn() {
        if (!gameStarted || gameOver) return;

        endTurn();
        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
        turnCount++;

        std::cout << "\n=== TURN " << turnCount << " ===\n";
        std::cout << getCurrentPlayer()->getName() << "'s turn begins.\n";

        drawCard();
        displayTurnInfo();
    }

    void Game::endTurn() {
        if (currentCard)
            returnCardToBottom();
        currentCard = nullptr;
    }

    bool Game::returnCardToBottom() {
        if (!currentCard) return false;
        getCurrentPlayer()->returnCardToBottom(currentCard);
        return true;
    }
    bool Game::performAction(const std::string& actionType, const std::string& target) {
        if (!currentCard || gameOver) return false;

        AgentType agentType = currentCard->getType();

        if (!isActionAllowed(agentType, actionType)) {
            std::cout << "ERROR: Action not allowed for this agent\n";
            return false;
        }

        bool success = false;

        if (actionType == "move")
            success = movePiece(agentType, target);
        else if (actionType == "attack")
            success = attackPiece(agentType, target);
        else if (actionType == "scout")
            success = scoutTile(target);
        else if (actionType == "control")
            success = controlTile(target);
        else if (actionType == "release")
            success = releaseTile(target);

        // 🔑 TURN END LOGIC (PDF-CORRECT)
        bool endTurn = false;

        if (actionType == "attack")
            endTurn = true;          // attack ALWAYS ends turn
        else
            endTurn = success;       // others only if successful

        if (endTurn) {
            checkWinConditions();
            if (!gameOver)
                nextTurn();
        }

        return success;
    }

    // --------------------------------------------------
    // ACTION VALIDATION
    // --------------------------------------------------
    bool Game::isActionAllowed(AgentType agentType, const std::string& actionType) {
        switch (agentType) {
        case AgentType::Scout:
            return actionType == "move" || actionType == "scout" || actionType == "attack";
        case AgentType::Sniper:
            return actionType == "move" || actionType == "attack";
        case AgentType::Seargeant:
            return actionType == "move" || actionType == "attack"
                   || actionType == "control" || actionType == "release";
        default:
            return false;
        }
    }

    // --------------------------------------------------
    // MOVE
    // --------------------------------------------------
    bool Game::movePiece(AgentType agentType, const std::string& targetCoord) {
        Player* current = getCurrentPlayer();
        Unit* piece = current->getAgentPiece(agentType);
        if (!piece || !piece->getPosition()) return false;

        Cell* from = piece->getPosition();
        Cell* to = nullptr;

        for (Cell* n : from->neighbors) {
            if (n && n->tileId.toStdString() == targetCoord) {
                to = n;
                break;
            }
        }

        if (!to || to->isOccupied()) return false;
        if (agentType != AgentType::Scout && !to->marked) return false;

        from->clearAgent();
        to->setAgent(agentType, current->getId() == 1 ? Side::A : Side::B);
        piece->setPosition(to);
        return true;
    }

    // --------------------------------------------------
    // SCOUT
    // --------------------------------------------------
    bool Game::scoutTile(const std::string&) {
        Player* current = getCurrentPlayer();
        Unit* scout = current->getAgentPiece(AgentType::Scout);
        if (!scout || !scout->getPosition()) return false;

        Cell* cell = scout->getPosition();
        if (cell->marked) return false;

        cell->marked = true;
        cell->markedBy = (current->getId() == 1 ? Side::A : Side::B);
        return true;
    }

    // --------------------------------------------------
    // ATTACK
    // --------------------------------------------------
    bool Game::attackPiece(AgentType agentType, const std::string&) {
        return CombatSystem::performAttack(
            agentType,
            getCurrentPlayer(),
            getOpponent()
            );
    }

    // --------------------------------------------------
    // CONTROL
    // --------------------------------------------------
    bool Game::controlTile(const std::string&) {
        Player* current = getCurrentPlayer();
        Unit* sergeant = current->getAgentPiece(AgentType::Seargeant);
        if (!sergeant || !sergeant->getPosition()) return false;

        Cell* cell = sergeant->getPosition();
        if (cell->controlledBy != Side::None) return false;

        cell->controlledBy = (current->getId() == 1 ? Side::A : Side::B);
        return true;
    }

    // --------------------------------------------------
    // RELEASE
    // --------------------------------------------------
    bool Game::releaseTile(const std::string&) {
        Player* current = getCurrentPlayer();
        Unit* sergeant = current->getAgentPiece(AgentType::Seargeant);
        if (!sergeant || !sergeant->getPosition()) return false;

        Cell* cell = sergeant->getPosition();
        Side mySide = (current->getId() == 1 ? Side::A : Side::B);

        if (cell->controlledBy == Side::None || cell->controlledBy == mySide)
            return false;

        cell->controlledBy = Side::None;
        return true;
    }

    // --------------------------------------------------
    // WIN CONDITIONS
    // --------------------------------------------------
    void Game::checkWinConditions() {
        Player* current = getCurrentPlayer();
        Player* opponent = getOpponent();

        if (opponent->getPieces().empty()) {
            gameOver = true;
            std::cout << "\n[VICTORY] "
                      << current->getName()
                      << " wins by eliminating all opponent pieces!\n";
            std::cout << "GAME OVER\n";
            return;
        }

        if (countControlledHouses(current) >= 7) {
            gameOver = true;
            std::cout << "\n[VICTORY] "
                      << current->getName()
                      << " wins by controlling 7 houses!\n";
            std::cout << "GAME OVER\n";
        }
    }

    int Game::countControlledHouses(Player* player) const {
        Side side = (player->getId() == 1 ? Side::A : Side::B);
        int count = 0;

        for (int r = 0; r < board.rows; ++r)
            for (int c = 0; c < board.cols; ++c)
                if (board.grid[r][c].type == 2 &&
                    board.grid[r][c].controlledBy == side)
                    count++;

        return count;
    }

    bool Game::isGameOver() const {
        return gameOver;
    }

    Player* Game::getCurrentPlayer() const {
        return players[currentPlayerIndex].get();
    }

    Player* Game::getOpponent() const {
        return players[(currentPlayerIndex + 1) % 2].get();
    }

    void Game::displayGameState() const {
        std::cout << "\n=== GAME STATE ===\n";
        for (const auto& p : players)
            p->displayStatus();
    }

    void Game::displayTurnInfo() const {
        std::cout << "\n--- CURRENT TURN ---\n";
        currentCard->display();
    }

    bool Game::loadMap(const std::string& path)
    {
        QString error;
        MapData data;

        if (!MapLoader::loadFromFile(QString::fromStdString(path),
                                     data, error))
            return false;

        board = data.board;
        return true;
    }

    const Board& Game::getBoard() const
    {
        return board;
    }
