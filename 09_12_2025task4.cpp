#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

int flood_time(const string& maze_str) {
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
    queue<tuple<int, int, int>> water_cells;
    int land_count = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            maze[i][j] = rows[i][j];
            if (maze[i][j] == 'W') {
                water_cells.push({i, j, 0});
            } else if (maze[i][j] == 'L') {
                land_count++;
            }
        }
    }
    
    if (land_count == 0) {
        return 0;
    }

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int max_time = 0;
    
    while (!water_cells.empty()) {
        auto [x, y, t] = water_cells.front();
        water_cells.pop();
        
        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                if (maze[nx][ny] == 'L' && !visited[nx][ny]) {
                    visited[nx][ny] = true;
                    land_count--;
                    water_cells.push({nx, ny, t + 1});
                    max_time = max(max_time, t + 1);
                    
                    // Меняем на воду для наглядности
                    maze[nx][ny] = 'W';
                }
            }
        }
        
        if (land_count == 0) {
            break;
        }
    }
    
    cout << "Финальное состояние:" << endl;
    for (const auto& row : maze) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
    
    return max_time;
}

int main() {
    string M = "WWWWWW\n"
               "WLLLLW\n"
               "WLLLLW\n"
               "WLLLLW\n"
               "WWWWWW";
    
    int time = flood_time(M);
    cout << "Время до полного затопления: " << time << " шагов" << endl;
    
    return 0;
}
