#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <cstring>

using namespace std;

vector<vector<int>> bfs(const vector<vector<char>>& maze, pair<int, int> start) {
    int n = maze.size();
    int m = maze[0].size();
    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));
    queue<pair<int, int>> q;
    
    q.push(start);
    dist[start.first][start.second] = 0;
    
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                if (maze[nx][ny] != '#' && dist[nx][ny] == INT_MAX) {
                    dist[nx][ny] = dist[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }
    
    return dist;
}

vector<vector<char>> find_meeting_point(const string& maze_str) {
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
    pair<int, int> A_pos, B_pos, F_pos;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            maze[i][j] = rows[i][j];
            if (maze[i][j] == 'A') A_pos = {i, j};
            if (maze[i][j] == 'B') B_pos = {i, j};
            if (maze[i][j] == 'F') F_pos = {i, j};
        }
    }
    
    auto dist_A = bfs(maze, A_pos);
    auto dist_B = bfs(maze, B_pos);
    auto dist_F = bfs(maze, F_pos);
    
    int min_cost = INT_MAX;
    pair<int, int> M_pos = F_pos;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (maze[i][j] != '#') {
                if (dist_A[i][j] != INT_MAX && dist_B[i][j] != INT_MAX && dist_F[i][j] != INT_MAX) {
                    int cost = dist_A[i][j] + dist_B[i][j] + dist_F[i][j];
                    if (cost < min_cost) {
                        min_cost = cost;
                        M_pos = {i, j};
                    }
                }
            }
        }
    }
    
    auto get_path = [&](const vector<vector<int>>& dist, pair<int, int> start, pair<int, int> end) {
        vector<vector<bool>> path(n, vector<bool>(m, false));
        pair<int, int> current = end;
        
        while (current != start) {
            auto [x, y] = current;
            path[x][y] = true;
            
            bool found = false;
            for (auto [dx, dy] : vector<pair<int, int>>{{0,1},{1,0},{0,-1},{-1,0}}) {
                int nx = x + dx;
                int ny = y + dy;
                
                if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                    if (dist[nx][ny] == dist[x][y] - 1) {
                        current = {nx, ny};
                        found = true;
                        break;
                    }
                }
            }
            if (!found) break;
        }
        return path;
    };
    
    auto path_A = get_path(dist_A, A_pos, M_pos);
    auto path_B = get_path(dist_B, B_pos, M_pos);
    auto path_M = get_path(dist_F, M_pos, F_pos);
    
    vector<vector<char>> result = maze;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if ((path_A[i][j] || path_B[i][j] || path_M[i][j]) && 
                result[i][j] != 'A' && result[i][j] != 'B' && result[i][j] != 'F') {
                result[i][j] = 'o';
            }
        }
    }
    
    if (M_pos != F_pos) {
        result[M_pos.first][M_pos.second] = 'M';
    }
    
    return result;
}

int main() {
    string M = "#######\n"
               "#A    #\n"
               "#     #\n"
               "#  F  #\n"
               "#     #\n"
               "#    B#\n"
               "#######";
    
    auto result = find_meeting_point(M);
    
    for (const auto& row : result) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
    
    return 0;
}
