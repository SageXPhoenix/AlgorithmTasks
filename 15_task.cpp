#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>

using namespace std;

class GameBoard {
public:
    vector<int> tiles;
    int movesCount;
    int estimatedCost;
    int combinedCost;
    int blankPosition;
    string direction;
    shared_ptr<GameBoard> parentBoard;
    
    GameBoard(const vector<int>& tileLayout, int moves = 0, shared_ptr<GameBoard> parent = nullptr, string dir = "")
        : tiles(tileLayout), movesCount(moves), parentBoard(parent), direction(dir) {
        blankPosition = -1;
        for (int idx = 0; idx < 16; idx++) {
            if (tiles[idx] == 0) {
                blankPosition = idx;
                break;
            }
        }
        estimatedCost = calculateManhattan();
        combinedCost = movesCount + estimatedCost;
    }
    
    bool operator>(const GameBoard& other) const {
        return combinedCost > other.combinedCost;
    }
    
    bool operator==(const GameBoard& other) const {
        return tiles == other.tiles;
    }
    
private:
    int calculateManhattan() const {
        int distanceSum = 0;
        for (int pos = 0; pos < 16; pos++) {
            int tileNumber = tiles[pos];
            if (tileNumber != 0) {
                int correctPosition = tileNumber - 1;
                int currentRow = pos / 4;
                int currentCol = pos % 4;
                int targetRow = correctPosition / 4;
                int targetCol = correctPosition % 4;
                distanceSum += abs(currentRow - targetRow) + abs(currentCol - targetCol);
            }
        }
        return distanceSum;
    }
};

struct BoardHash {
    size_t operator()(const GameBoard& board) const {
        size_t hash = 0;
        for (int tile : board.tiles) {
            hash = hash * 31 + tile;
        }
        return hash;
    }
};

struct BoardComparator {
    bool operator()(const shared_ptr<GameBoard>& a, const shared_ptr<GameBoard>& b) const {
        return a->combinedCost > b->combinedCost;
    }
};

struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t hash = 0;
        for (int num : v) {
            hash = hash * 31 + num;
        }
        return hash;
    }
};

class GameSolver {
private:
    vector<int> solvedState = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
    
public:
    bool checkSolvability(const vector<int>& tiles) {
        return true;
    }
    
    vector<shared_ptr<GameBoard>> getPossibleMoves(shared_ptr<GameBoard> current) {
        vector<shared_ptr<GameBoard>> moves;
        int blankRow = current->blankPosition / 4;
        int blankCol = current->blankPosition % 4;
        
        vector<pair<int, int>> offsets = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        vector<string> directions = {"U", "D", "L", "R"};
        
        for (int i = 0; i < 4; i++) {
            int newRow = blankRow + offsets[i].first;
            int newCol = blankCol + offsets[i].second;
            
            if (newRow >= 0 && newRow < 4 && newCol >= 0 && newCol < 4) {
                int newPos = newRow * 4 + newCol;
                vector<int> newTiles = current->tiles;
                swap(newTiles[current->blankPosition], newTiles[newPos]);
                
                moves.push_back(make_shared<GameBoard>(
                    newTiles, current->movesCount + 1, current, directions[i]
                ));
            }
        }
        
        return moves;
    }
    
    bool isSolved(const vector<int>& tiles) {
        return tiles == solvedState;
    }
    
    vector<string> findSolutionPath(const vector<int>& startTiles) {
        if (!checkSolvability(startTiles)) {
            return {};
        }
        
        auto startBoard = make_shared<GameBoard>(startTiles);
        priority_queue<shared_ptr<GameBoard>, vector<shared_ptr<GameBoard>>, BoardComparator> openList;
        unordered_set<GameBoard, BoardHash> closedList;
        unordered_map<vector<int>, int, VectorHash> bestScores;
        
        openList.push(startBoard);
        bestScores[startTiles] = 0;
        
        while (!openList.empty()) {
            auto current = openList.top();
            openList.pop();
            
            if (closedList.count(*current)) {
                continue;
            }
            
            if (isSolved(current->tiles)) {
                return reconstructPath(current);
            }
            
            closedList.insert(*current);
            
            for (auto next : getPossibleMoves(current)) {
                if (closedList.count(*next)) {
                    continue;
                }
                
                auto it = bestScores.find(next->tiles);
                if (it == bestScores.end() || next->movesCount < it->second) {
                    bestScores[next->tiles] = next->movesCount;
                    openList.push(next);
                }
            }
        }
        
        return {};
    }
    
private:
    vector<string> reconstructPath(shared_ptr<GameBoard> finalBoard) {
        vector<string> path;
        auto current = finalBoard;
        
        while (current->parentBoard != nullptr) {
            path.push_back(current->direction);
            current = current->parentBoard;
        }
        
        reverse(path.begin(), path.end());
        return path;
    }
};

void printBoard(const vector<int>& board) {
    cout << "Current board:" << endl;
    for (int i = 0; i < 16; i++) {
        cout << (board[i] < 10 ? "0" : "") << board[i] << " ";
        if ((i + 1) % 4 == 0) {
            cout << endl;
        }
    }
}

int main() {
    //Конфигурация пятнашек
    vector<int> startTiles = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 14, 15};
    
    printBoard(startTiles);
    cout << endl;
    
    GameSolver solver;
    
    cout << "Finding solution..." << endl;
    auto solution = solver.findSolutionPath(startTiles);
    
    if (!solution.empty()) {
        cout << "Solution found in " << solution.size() << " moves:" << endl;
        for (size_t i = 0; i < solution.size(); i++) {
            cout << solution[i];
            if (i < solution.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    } else {
        cout << "No solution found." << endl;
    }
    
    return 0;
}
