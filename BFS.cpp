#include "BFS.h"
#include "Cell.h"

#include <queue>
#include <unordered_set>

std::vector<Cell*> BFS::traverse(
    Cell* start,
    std::function<bool(Cell*)> canVisit,
    std::function<void(Cell*)> onVisit
    ) {
    std::vector<Cell*> result;

    if (!start)
        return result;

    std::queue<Cell*> q;
    std::unordered_set<Cell*> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Cell* current = q.front();
        q.pop();

        if (canVisit && !canVisit(current))
            continue;

        result.push_back(current);

        if (onVisit)
            onVisit(current);

        for (Cell* neighbor : current->neighbors) {
            if (!neighbor)
                continue;

            if (visited.find(neighbor) != visited.end())
                continue;

            visited.insert(neighbor);
            q.push(neighbor);
        }
    }

    return result;
}
