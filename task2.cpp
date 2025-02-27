#include <iostream>
#include <vector>
using namespace std;

bool check(const vector<int>& ind, int k, int mid) {
    int j = 0, i = 1, ans = 1;
    while (i < ind.size()) {
        int delta = ind[i] - ind[j];
        if (delta > mid) {
            if (i - j == 1) return false;
            else {
                ans++;
                j = i - 1;
            }
        } else if (delta == mid) {
            ans++;
            j = i;
            i++;
        } else {
            if (i == ind.size()) ans++;
            i++;
        }
    }
    return ans <= k;
}

int bin_search(int n, int k, const vector<int>& ma, const vector<int>& ind) {
    int left = 0, right = n;
    while (right - left > 1) {
        int mid = (right + left) / 2;
        if (check(ind, k, mid)) right = mid;
        else left = mid;
    }
    if (check(ind, k, left)) return left;
    return right;
}

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> ma(n);
    for (int i = 0; i < n; i++) {
        cin >> ma[i];
    }
    vector<int> ind = {0};
    for (int i = 0; i < n - 1; i++) {
        if (ma[i]) ind.push_back(i + 1);
    }
    ind.push_back(n);
    cout << bin_search(n, k, ma, ind) << endl;
    return 0;
}
