#include <iostream>
#include <vector>
using namespace std;

vector<int> bin_search(int n, int m, const vector<int>& a, const vector<int>& b) {
    vector<int> answer;
    for (int x : b) {
        int left = 0, right = n - 1;
        while (right - left > 1) {
            int mid = (right + left) / 2;
            if (a[mid] < x) {
                left = mid;
            } else {
                right = mid;
            }
        }
        if (a[left] >= x) {
            answer.push_back(left);
        } else {
            answer.push_back(right);
        }
    }
    return answer;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < m; i++) {
        cin >> b[i];
    }
    vector<int> result = bin_search(n, m, a, b);
    for (int idx : result) {
        cout << idx << " ";
    }
    cout << endl;
    return 0;
}
