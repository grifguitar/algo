#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int pow2(int k) {
    return (1 << k);
}

void count_sort(vector<pair<int, int>> &pairs, vector<vector<int>> &count) {
    for (auto &x: count) x.resize(0);
    for (auto &pair: pairs)
        count[pair.first].push_back(pair.second);
    int cnt = 0;
    for (int i = 0; i < count.size(); i++)
        for (int j = 0; j < count[i].size(); j++)
            pairs[cnt++] = {i, count[i][j]};
}

void base_induction(string &s, vector<int> &arr, vector<int> &colors, vector<pair<int, int>> &pairs) {
    for (int i = 0; i < s.size(); i++)
        pairs[i] = {s[i], i};
    sort(pairs.begin(), pairs.end());
    arr[0] = pairs[0].second;
    colors[arr[0]] = 0;
    for (int i = 1; i < pairs.size(); i++) {
        arr[i] = pairs[i].second;
        colors[arr[i]] = colors[arr[i - 1]] + (int) (pairs[i].first != pairs[i - 1].first);
    }
}

void step_induction(string &s, vector<int> &arr, vector<int> &colors, vector<int> &new_colors,
                    vector<vector<int>> &count, vector<pair<int, int>> &pairs, int k) {
    for (int i = 0; i < s.size(); i++) {
        int num = (arr[i] - pow2(k - 1) + s.size()) % s.size();
        pairs[i] = {colors[num], num};
    }
    count_sort(pairs, count);
    arr[0] = pairs[0].second;
    new_colors[arr[0]] = 0;
    for (int i = 1; i < pairs.size(); i++) {
        arr[i] = pairs[i].second;
        new_colors[arr[i]] = new_colors[arr[i - 1]] + (int) (pairs[i].first != pairs[i - 1].first ||
                                                             colors[(arr[i] + pow2(k - 1)) % s.size()] !=
                                                             colors[(arr[i - 1] + pow2(k - 1)) % s.size()]);
    }
    swap(colors, new_colors);
}

void kasai(string &s, vector<int> &arr) {
    vector<int> lcp(s.size() - 1);
    vector<int> index(s.size());
    for (int i = 0; i < s.size(); i++)
        index[arr[i]] = i;
    int k = 0;
    for (int i = 0; i < s.size() - 1; i++) {
        int pos = index[i];
        if (k > 0) k--;
        while (s[(arr[pos - 1] + k)] == s[(arr[pos] + k)]) k++;
        lcp[pos - 1] = k;
    }
    lcp.erase(lcp.begin());
    for (int &x: lcp) cout << x << " ";
}

int main() {

    string s;
    cin >> s;
    s += "$";

    vector<int> arr(s.size());
    vector<int> colors(s.size());
    vector<int> new_colors(s.size());
    vector<pair<int, int>> pairs(s.size());
    vector<vector<int>> count(s.size());

    base_induction(s, arr, colors, pairs);

    for (int k = 1; pow2(k - 1) <= s.size(); k++) {
        step_induction(s, arr, colors, new_colors, count, pairs, k);
    }

    for (int i = 0; i < s.size() - 1; i++) {
        cout << arr[i + 1] + 1 << " ";
    }
    cout << "\n";

    kasai(s, arr);

    return 0;
}
