#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cstring>

using namespace std;

vector<vector<char>> find_exit(const string& maze_str) {
    vector<string> rows;
    size_t pos = 0;
    string copy = maze_str;
    
    while ((pos = copy.find('\n')) != string::npos) {
        if (pos > 0) {
            rows.push_back(copy.substr(0, pos));
        }
        copy.erase(0, pos + 1);
    }
    if (!copy.empty()) {
        rows.push_back(copy);
    }
    
    int n = rows.size();
    int m = rows[0].size();
    
    vector<vector<char>> maze(n, vector<char>(m));
    pair<int, int> start;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            maze[i][j] = rows[i][j];
            if (maze[i][j] == 'S') {
                start = {i, j};
            }
        }
    }
    
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1, -1}));
    queue<pair<int, int>> q;
    
    q.push(start);
    visited[start.first][start.second] = true;
    
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    pair<int, int> exit_pos = {-1, -1};
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        if (x == 0 || x == n-1 || y == 0 || y == m-1) {
            exit_pos = {x, y};
            break;
        }
        
        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                if (maze[nx][ny] == ' ' && !visited[nx][ny]) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                    parent[nx][ny] = {x, y};
                }
            }
        }
    }
    
    if (exit_pos.first != -1) {
        vector<vector<bool>> path_cells(n, vector<bool>(m, false));
        pair<int, int> current = exit_pos;
        
        while (current != start) {
            auto [x, y] = current;
            path_cells[x][y] = true;
            current = parent[x][y];
            if (current.first == -1) break
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (path_cells[i][j] && (i != start.first || j != start.second)) {
                    maze[i][j] = 'o';
                }
            }
        }
    }
    
    return maze;
}

int main() {
    string M = "#####\n"
               "# S #\n"
               "#   #\n"
               "#   #\n"
               "#####";
    
    auto result = find_exit(M);
    
    for (const auto& row : result) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
    
    return 0;
}
