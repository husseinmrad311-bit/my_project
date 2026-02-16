    #include "Game.h"
    #include "CombatSystem.h"
    #include "MapLoader.h"
    #include <iostream>
    #include <QFile>
    #include <QTextStream>
    #include <QStringList>
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

        synchronizeUnitsWithBoard();

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
        checkWinConditions();
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

        // TURN END LOGIC
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
    bool Game::isActionAllowed(AgentType agentType, const std::string& actionType) {
        std::cout << "isActionAllowed - Agent: "
                  << (agentType == AgentType::Scout ? "Scout" :
                          agentType == AgentType::Sniper ? "Sniper" : "Sergeant")
                  << ", Action: " << actionType << std::endl;

        switch (agentType) {
        case AgentType::Scout:
            return actionType == "move" || actionType == "scout" || actionType == "attack";
        case AgentType::Sniper:
            return actionType == "move" || actionType == "attack";
        case AgentType::Seargeant:
            return actionType == "move" || actionType == "attack"
                   || actionType == "control" || actionType == "release";
        default:
            std::cout << "Unknown agent type!" << std::endl;
            return false;
        }
    }

    // --------------------------------------------------
    // MOVE
    // --------------------------------------------------
    bool Game::movePiece(AgentType agentType, const std::string& targetCoord)
    {
        // ===== VALIDATION CHECKS =====
        Player* current = getCurrentPlayer();
        if (!current) {
            std::cout << "ERROR: No current player\n";
            return false;
        }

        Unit* piece = current->getAgentPiece(agentType);
        if (!piece) {
            std::cout << "ERROR: No piece found for agent type\n";
            return false;
        }

        Cell* from = piece->getPosition();
        if (!from) {
            std::cout << "ERROR: Piece has no position (not placed on board)\n";
            return false;
        }

        // Validate that 'from' cell belongs to this board
        bool fromValid = false;
        for (int r = 0; r < board.rows && !fromValid; ++r) {
            for (int c = 0; c < board.cols && !fromValid; ++c) {
                if (from == &board.grid[r][c]) {
                    fromValid = true;
                }
            }
        }
        if (!fromValid) {
            std::cout << "ERROR: From cell does not belong to current board!\n";
            return false;
        }

        // ===== FIND TARGET CELL =====
        Cell* to = nullptr;

        if (targetCoord.empty()) {
            std::cout << "ERROR: Empty target coordinate\n";
            return false;
        }

        // First, find target cell in board
        Cell* targetCell = nullptr;
        for (int r = 0; r < board.rows; ++r) {
            for (int c = 0; c < board.cols; ++c) {
                if (board.grid[r][c].tileId.toStdString() == targetCoord) {
                    targetCell = &board.grid[r][c];
                    break;
                }
            }
        }

        if (!targetCell) {
            std::cout << "ERROR: Target cell " << targetCoord << " not found in board\n";
            return false;
        }

        // Now check if target is a neighbor of from
        bool isNeighbor = false;
        for (Cell* neighbor : from->neighbors) {
            if (neighbor == targetCell) {
                isNeighbor = true;
                to = targetCell;
                break;
            }
        }

        if (!isNeighbor) {
            std::cout << "Move failed: target " << targetCoord
                      << " is not adjacent to " << from->tileId.toStdString() << "\n";
            return false;
        }

        // ===== VALIDATE TARGET CELL =====

        if (to->isOccupied()) {
            std::cout << "Move failed: tile " << targetCoord << " is occupied\n";
            return false;
        }

        if (agentType != AgentType::Scout) {
            if (!to->marked) {
                std::cout << "Move failed: tile " << targetCoord
                          << " is not marked (scouted)\n";
                return false;
            }

            Side mySide = (current->getId() == 1 ? Side::A : Side::B);
            if (to->markedBy != mySide) {
                std::cout << "Move failed: tile " << targetCoord
                          << " is marked by opponent\n";
                return false;
            }
        }

        // ===== PERFORM MOVEMENT SAFELY =====
        try {
            // 1. Clear origin cell
            from->clearAgent();

            // 2. Set destination cell
            Side mySide = (current->getId() == 1 ? Side::A : Side::B);
            to->setAgent(agentType, mySide);

            // 3. Update unit's position
            piece->setPosition(to);

            std::cout << "Move successful: "
                      << piece->getTypeName() << " moved from "
                      << from->tileId.toStdString() << " to "
                      << to->tileId.toStdString() << "\n";

            return true;

        } catch (const std::exception& e) {
            std::cout << "EXCEPTION during move: " << e.what() << "\n";
            return false;
        }
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
    bool Game::attackPiece(AgentType agentType, const std::string& targetCoord)
    {
        Player* attacker = getCurrentPlayer();
        Player* defender = getOpponent();

        Unit* attackerUnit = attacker->getAgentPiece(agentType);
        if (!attackerUnit || !attackerUnit->getPosition())
            return false;

        // Find target cell
        Cell* targetCell = nullptr;

        for (int r = 0; r < board.rows; ++r)
        {
            for (int c = 0; c < board.cols; ++c)
            {
                if (board.grid[r][c].tileId.toStdString() == targetCoord)
                {
                    targetCell = &board.grid[r][c];
                    break;
                }
            }
        }

        if (!targetCell)
            return false;

        Side mySide = (attacker->getId() == 1 ? Side::A : Side::B);
        Side enemySide = (mySide == Side::A ? Side::B : Side::A);

        // Must contain enemy unit
        if (!targetCell->isOccupiedBy(enemySide)) {
            std::cout << "No enemy unit at target\n";
            return false;
        }

        std::cout << "Target unit type: "
                  << (targetCell->agent == AgentType::Scout ? "Scout" :
                          targetCell->agent == AgentType::Sniper ? "Sniper" : "Sergeant")
                  << "\n";

        return CombatSystem::performAttack(
            agentType,
            attacker,
            defender,
            targetCell
            );
    }

    // --------------------------------------------------
    // CONTROL
    // --------------------------------------------------
    bool Game::controlTile(const std::string&)
    {
        Player* current = getCurrentPlayer();
        Player* opponent = getOpponent();

        Unit* sergeant = current->getAgentPiece(AgentType::Seargeant);
        if (!sergeant || !sergeant->getPosition())
            return false;

        Cell* cell = sergeant->getPosition();

        Side mySide = (current->getId() == 1 ? Side::A : Side::B);
        Side opponentSide = (mySide == Side::A ? Side::B : Side::A);

        // Must not already be controlled
        if (cell->controlledBy != Side::None)
            return false;

        // MUST NOT contain enemy piece
        if (cell->isOccupiedBy(opponentSide))
            return false;

        cell->controlledBy = mySide;
        return true;
    }

    // --------------------------------------------------
    // RELEASE
    // --------------------------------------------------
    bool Game::releaseTile(const std::string&)
    {
        Player* current = getCurrentPlayer();
        Player* opponent = getOpponent();

        Unit* sergeant = current->getAgentPiece(AgentType::Seargeant);
        if (!sergeant || !sergeant->getPosition())
            return false;

        Cell* cell = sergeant->getPosition();

        Side mySide = (current->getId() == 1 ? Side::A : Side::B);
        Side opponentSide = (mySide == Side::A ? Side::B : Side::A);

        // Must be controlled by enemy
        if (cell->controlledBy != opponentSide)
            return false;

        //  Must not contain enemy piece
        if (cell->isOccupiedBy(opponentSide))
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

        // IMPORTANT: Use move, not copy!
        board = std::move(data.board);  // This uses move assignment
        // board = data.board;           //  This tries to use deleted copy assignment

        return true;
    }

    const Board& Game::getBoard() const
    {
        return board;
    }

    void Game::synchronizeUnitsWithBoard()
    {
        // First, clear all unit positions
        for (auto& player : players) {
            for (auto& piece : player->getPieces()) {
                piece->setPosition(nullptr);
            }
        }

        // Then set them from board
        for (int r = 0; r < board.rows; ++r)
        {
            for (int c = 0; c < board.cols; ++c)
            {
                Cell& cell = board.grid[r][c];

                if (cell.agent == AgentType::None)
                    continue;

                Player* player = (cell.agentSide == Side::A) ? players[0].get() : players[1].get();

                Unit* unit = player->getAgentPiece(cell.agent);
                if (unit) {
                    unit->setPosition(&cell);
                    std::cout << "  Synced " << unit->getTypeName()
                              << " [P" << player->getId() << "] to "
                              << cell.tileId.toStdString() << std::endl;
                }
            }
        }
    }



    bool Game::loadStateFile(const std::string& path)
    {
        std::cout << "\n=== LOADING STATE FILE: " << path << " ===\n";

        QString error;
        bool success = MapLoader::loadStateFile(
            QString::fromStdString(path),
            board,
            error
            );

        if (!success) {
            std::cout << "Failed to load state file: " << error.toStdString() << std::endl;
            return false;
        }

        // Synchronize units with new board positions
        std::cout << "Synchronizing units with board...\n";
        synchronizeUnitsWithBoard();

        // Verify the synchronization
        for (const auto& player : players) {
            for (const auto& piece : player->getPieces()) {
                Cell* pos = piece->getPosition();
                if (pos) {
                    std::cout << "  " << piece->getTypeName()
                    << " [P" << player->getId() << "] at "
                    << pos->tileId.toStdString() << std::endl;
                } else {
                    std::cout << "  WARNING: " << piece->getTypeName()
                    << " [P" << player->getId() << "] has no position!" << std::endl;
                }
            }
        }

        std::cout << "=== STATE FILE LOADED ===\n\n";
        return true;
    }

