#include <vector>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <string>

using namespace std;

class Solution {
public:
    int shortestPathAllKeys(vector<string>& grid) {
        int m = grid.size(), n = grid[0].size();
        int start_x = -1, start_y = -1;
        int key_count = 0;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                char c = grid[i][j];
                if (c == '@') {
                    start_x = i;
                    start_y = j;
                } else if (c >= 'a' && c <= 'z') {
                    key_count++;
                }
            }
        }
        
        vector<vector<int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        unordered_set<string> visited;
        queue<tuple<int, int, int>> q;
        
        int all_keys_mask = (1 << key_count) - 1;
        q.push({start_x, start_y, 0});
        visited.insert(to_string(start_x) + "," + to_string(start_y) + ",0");
        
        int steps = 0;
        
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                auto [x, y, mask] = q.front();
                q.pop();
                
                if (mask == all_keys_mask) {
                    return steps;
                }
                
                for (auto& dir : dirs) {
                    int nx = x + dir[0];
                    int ny = y + dir[1];
                    int new_mask = mask;
                    
                    if (nx < 0 || nx >= m || ny < 0 || ny >= n) {
                        continue;
                    }
                    
                    char cell = grid[nx][ny];
                    
                    if (cell == '#') {
                        continue;
                    }
                    
                    if (cell >= 'A' && cell <= 'Z') {
                        int key_bit = 1 << (cell - 'A');
                        if (!(new_mask & key_bit)) {
                            continue;
                        }
                    }
                    
                    if (cell >= 'a' && cell <= 'z') {
                        int key_bit = 1 << (cell - 'a');
                        new_mask |= key_bit;
                    }
                    
                    string state = to_string(nx) + "," + to_string(ny) + "," + to_string(new_mask);
                    if (!visited.count(state)) {
                        visited.insert(state);
                        q.push({nx, ny, new_mask});
                    }
                }
            }
            steps++;
        }
        
        return -1;
    }
};
