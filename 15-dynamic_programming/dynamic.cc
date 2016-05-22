#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int MemeizedCutRodAux(int r[], int s[], int p[], int n);
void PrintOptimalParens(int *s, int n, int i, int j);
void LcsPrint(int *source, int *solution, int n, int length_a, int length_b);
void PrintOptimalBst(int *root, int n, int i, int j, int last_root);

void PrintArray(int *src, int a, int b) {
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < b; ++j) {
      cout << *(src + i * a + j) << " ";
    }
    cout << endl;
  }
}
void ClearArray(int *src, int a, int b, int fill) {
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < b; ++j) {
      *(src + a * i + j) = fill;
    }
  }
}

/********************************** the rod cut question
 * ************************************/
void MemeizedCutRod(int p[], int n) {
  int r[n + 1], s[n + 1];
  for (int i = 0; i < n + 1; i++) {
    r[i] = s[i] = 0;
  }
  cout << "the max profit:" << MemeizedCutRodAux(r, s, p, n) << endl;
  cout << "the split node:";
  int length = n;
  while (length > 0) {
    cout << s[length] << " ";
    length -= s[length];
  }
  cout << endl;
}

int MemeizedCutRodAux(int r[], int s[], int p[], int n) {
  if (r[n] > 0) {
    return r[n];
  }

  int q = INT_MIN;
  if (0 == n) {
    q = 0;
  } else {
    for (int i = 1; i <= n; i++) {
      int tmp = p[i] + MemeizedCutRodAux(r, s, p, n - i);
      if (q < tmp) {
        q = tmp;
        s[n] = i;
      }
    }
  }
  r[n] = q;
  return q;
}

void BottomUpCutRod(int p[], int n) {
  int r[n + 1], s[n + 1];
  r[0] = 0;
  for (int j = 1; j <= n; j++) {
    int q = INT_MIN;
    for (int i = 1; i <= j; i++) {
      if (q < p[i] + r[j - i]) {
        q = p[i] + r[j - i];
        s[j] = i;
        r[j] = q;
      }
    }
  }
  cout << "the max profit:" << r[n - 1] << endl;
  cout << "the split node:";
  int length = n;
  while (length > 0) {
    cout << s[length] << " ";
    length -= s[length];
  }
  cout << endl;
}

/****************************************** matrix multiply chain question
 * ***************************/
void MatrixChainOrder(int p[], int n) {
  int m[n][n], s[n][n];
  memset(m, 0, sizeof(m));
  memset(s, 0, sizeof(s));
  for (int l = 2; l <= n; ++l) {
    for (int i = 0; i <= n - l; ++i) {
      // closed interval
      int j = i + l - 1;
      m[i][j] = INT_MAX;
      for (int k = i; k <= j; ++k) {
        int cost = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
        if (cost < m[i][j]) {
          m[i][j] = cost;
          s[i][j] = k;
        }
      }
    }
  }
  PrintOptimalParens(&s[0][0], n, 0, n - 1);
  cout << endl;
}

void PrintOptimalParens(int *s, int n, int i, int j) {
  if (i == j) {
    cout << "A" << i;
  } else {
    cout << "(";
    PrintOptimalParens(s, n, i, *(s + n * i + j));
    PrintOptimalParens(s, n, *(s + n * i + j) + 1, j);
    cout << ")";
  }
}

/******************************************* LCS problem
 * *******************************************/
void LcsLength(int *lcs_a, int length_a, int *lcs_b, int length_b) {
  int result[length_a + 1][length_b + 1], solution[length_a + 1][length_b + 1];
  const int kLeft = 0, kUp = 1, kLeft_Up = 2;

  for (int i = 0; i <= length_a; i++) {
    result[i][0] = 0;
  }
  for (int i = 0; i <= length_b; i++) {
    result[0][i] = 0;
  }

  for (int i = 0; i <= length_a; i++) {
    for (int j = 0; j <= length_b; j++) {
      solution[i][j] = 0;
    }
  }
  for (int i = 1; i <= length_a; i++) {
    for (int j = 1; j <= length_b; j++) {
      if (lcs_a[i] == lcs_b[j]) {
        result[i][j] = result[i - 1][j - 1] + 1;
        solution[i][j] = kLeft_Up;
        // cout << i << "re" << j << endl;
      } else if (result[i][j - 1] <= result[i - 1][j]) {
        result[i][j] = result[i - 1][j];
        solution[i][j] = kUp;
        // cout << i << "," << j << endl;
      } else {
        result[i][j] = result[i][j - 1];
        solution[i][j] = kLeft;
        // cout << i << "hre" << j << endl;
      }
    }
  }
  cout << "longest:" << result[length_a][length_b] << endl;
  LcsPrint(lcs_a, &solution[0][0], length_b + 1, length_a, length_b);
}

void LcsPrint(int *source, int *solution, int n, int length_a, int length_b) {
  const int kLeft = 0, kUp = 1, kLeft_Up = 2;
  if (length_a == 0 || length_b == 0) {
    return;
  } else if (*(solution + n * length_a + length_b) == kLeft_Up) {
    LcsPrint(source, solution, n, length_a - 1, length_b - 1);
    cout << source[length_a] << " ";
  } else if (*(solution + n * length_a + length_b) == kLeft) {
    LcsPrint(source, solution, n, length_a, length_b - 1);
  } else {
    LcsPrint(source, solution, n, length_a - 1, length_b);
  }
}

void LcsLengthComprass(int *lcs_a, int length_a, int *lcs_b, int length_b) {
  if (length_a < length_b) {
    LcsLengthComprass(lcs_b, length_b, lcs_a, length_a);
  }

  int result[length_b + 1];
  for (int i = 0; i <= length_b; i++) {
    result[i] = 0;
  }
  int last_left_up = 0;

  for (int i = 1; i <= length_a; i++) {
    for (int j = 1; j <= length_b; j++) {
      if (lcs_a[i] == lcs_b[j]) {
        int tmp = last_left_up;
        last_left_up = result[j];
        result[j] = tmp + 1;
      } else if (result[j] <= result[j - 1]) {
        last_left_up = result[j];
        result[j] = result[j - 1];
      } else {
        last_left_up = result[j];
      }
    }
    last_left_up = 0;
  }
  cout << "longeset:" << result[length_b - 1] << endl;
}

/********************************* longeset increase subsequance problem
 * **************************/
void LIS(vector<int> src) {
  vector<int> tmp(src);
  sort(src.begin(), src.end());

  // then figure the LCS of tmp and src
}

void LisBetter();

/****************************** optimal bst
 * ******************************************/
void OptimalBst(const vector<float> p, const vector<float> q, int n) {
  float e[n + 2][n + 1], w[n + 2][n + 1];
  int root[n + 1][n + 1];
  memset(root, 0, sizeof(root));
  for (int i = 1; i <= n + 1; i++) {
    e[i][i - 1] = q[i - 1];
    w[i][i - 1] = q[i - 1];
  }
  for (int l = 1; l <= n; l++) {
    for (int i = 1; i <= n - l + 1; ++i) {
      int j = i + l - 1;
      e[i][j] = 10e10;
      w[i][j] = w[i][j - 1] + p[j] + q[j];
      for (int r = i; r <= j; ++r) {
        float t = e[i][r - 1] + e[r + 1][j] + w[i][j];
        if (t < e[i][j]) {
          e[i][j] = t;
          root[i][j] = r;
        }
      }
    }
  }
  PrintOptimalBst(&root[0][0], n + 1, 1, n, 0);
}

void OptimalBstBetter(const vector<float> p, const vector<float> q, int n) {
  float e[n + 2][n + 1], w[n + 2][n + 1];
  int root[n + 1][n + 1];
  memset(root, 0, sizeof(root));
  for (int i = 1; i <= n + 1; i++) {
    e[i][i - 1] = q[i - 1];
    w[i][i - 1] = q[i - 1];
    root[i][i - 1] = i;
  }
  for (int l = 1; l <= n; l++) {
    for (int i = 1; i <= n - l + 1; ++i) {
      int j = i + l - 1;
      e[i][j] = 10e10;
      w[i][j] = w[i][j - 1] + p[j] + q[j];
      // for (int r = root[i][j-1];r <= root[i+1][j];++r) {
      for (int r = max(i, root[i][j - 1]); r <= min(j, root[i + 1][j]); ++r) {
        float t = e[i][r - 1] + e[r + 1][j] + w[i][j];
        if (t < e[i][j]) {
          e[i][j] = t;
          root[i][j] = r;
        }
      }
    }
  }
  PrintOptimalBst(&root[0][0], n + 1, 1, n, 0);
}

void PrintOptimalBst(int *root, int n, int i, int j, int last_root) {
  if (i == 1 && j == n - 1) {
    cout << "k" << *(root + n * i + j) << " is the root" << endl;
    PrintOptimalBst(root, n, i, *(root + n * i + j) - 1, *(root + n * i + j));
    PrintOptimalBst(root, n, *(root + n * i + j) + 1, j, *(root + n * i + j));
    return;
  }

  if (i == j + 1) {
    cout << "d" << j << " is the " << (i % 2 ? "left" : "right") << " kid of k"
         << last_root << endl;
  } else {
    cout << "k" << *(root + n * i + j) << " is the "
         << (i < last_root ? "left" : "right") << " kid of k" << last_root
         << endl;
    PrintOptimalBst(root, n, i, *(root + n * i + j) - 1, *(root + n * i + j));
    PrintOptimalBst(root, n, *(root + n * i + j) + 1, j, *(root + n * i + j));
  }
}

/*********************************** longest palindrome subsequance problem
 * *************************/
void LPS(char *str, int length) {
  int r[length + 1][length + 1];
  for (int i = 0; i <= length; ++i) {
    for (int j = 0; j <= length; ++j) {
      r[i][j] = 0;
    }
  }

  for (int i = 1; i <= length; ++i) {
    r[i][i] = 1;
  }

  for (int l = 1; l <= length; ++l) {
    for (int i = 1; i + l - 1 <= length; ++i) {
      int j = i + l - 1;
      if (i == j) {
        r[i][j] = 1;
        continue;
      }
      if (str[i - 1] == str[j - 1]) {
        r[i][j] = r[i + 1][j - 1] + 2;
      } else {
        if (r[i + 1][j] < r[i][j - 1]) {
          r[i][j] = r[i][j - 1];
        } else {
          r[i][j] = r[i + 1][j];
        }
      }
    }
  }

  cout << "the lps is:" << r[1][length] << endl;
}

/****************************** bitonic tours ************************/
typedef struct point {
  float x;
  float y;
} Point;

float Distance(Point a, Point b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void BitonicTours(vector<Point> p) {
  sort(p.begin(), p.end(), [](Point &a, Point &b) { return a.x < b.x; });

  int n = p.size();
  cout << n << endl;
  cout << p[n - 1].x << endl;
  float b[n + 1][n + 1];
  memset(b, 0, sizeof(b));

  b[1][2] = Distance(p[0], p[1]);
  for (int j = 3; j <= n; ++j) {
    for (int i = 1; i <= j - 2; ++i) {
      b[i][j] = b[i][j - 1] + Distance(p[j - 2], p[j - 1]);
    }
    b[j - 1][j] = 10e100;
    for (int k = 1; k <= j - 2; ++k) {
      float q = b[k][j - 1] + Distance(p[k - 1], p[j - 1]);
      if (q < b[j - 1][j]) {
        b[j - 1][j] = q;
      }
    }
  }
  b[n][n] = b[n - 1][n] + Distance(p[n - 2], p[n - 1]);

  cout << "BitonicTours:" << b[n][n] << endl;
}

/************************ print neatly problem ******************/
void PrintNeatly(vector<string> words, int max_size) {
  int n = words.size();
  int extra[n][n];
  int lc[n][n];
  int result[n];

  memset(result, 0, sizeof(result));
  ClearArray(&extra[0][0], n, n, 0);
  ClearArray(&lc[0][0], n, n, 10000);
  // ClearArray(&lc[0][0],n,n,0);
  PrintArray(&lc[0][0], n, n);
  for (int i = 0; i < n; ++i) {
    extra[i][i] = max_size - words[i].size();
    lc[i][i] = extra[i][i] * extra[i][i] * extra[i][i];
  }

  for (int l = 2; l <= n; ++l) {
    for (int i = 0; i + l - 1 < n; ++i) {
      int j = i + l - 1;
      extra[i][j] = extra[i][j - 1] - words[j].size() - 1;
      if (extra[i][j] >= 0) {
        if (j == n - 1) {
          lc[i][j] = 0;
        } else {
          lc[i][j] = extra[i][j] * extra[i][j] * extra[i][j];
        }
      }
    }
  }

  PrintArray(&extra[0][0], n, n);
  PrintArray(&lc[0][0], n, n);

  result[0] = 0;
  for (int j = 1; j < n; ++j) {
    result[j] = 10000;
    for (int i = 0; i <= j; ++i) {
      int tmp = result[i - 1] + lc[i][j];
      if (tmp < result[j]) {
        result[j] = tmp;
      }
    }
  }

  PrintArray(result, 1, n);
  cout << "the lc:" << result[n - 1] << endl;
}

int main() {
  /* int x[11]{0,1,5,8,9,10,17,17,20,24,30}; */
  // BottomUpCutRod(x,11);
  // cout << "tag" << endl;
  // MemeizedCutRod(x,10);

  // int matrix[]{30,35,15,5,10,20,25};
  // MatrixChainOrder(matrix,6);

  // int lcs_a[]{0,1,2,3,2,4,1,2};
  // int lcs_b[]{0,2,4,3,1,2,1};
  // LcsLength(lcs_a,7,lcs_b,6);
  // cout << endl;
  // LcsLengthComprass(lcs_a,7,lcs_b,6);

  // vector<float> p{0,0.15,0.10,0.05,0.10,0.20};
  // vector<float> q{0.05,0.10,0.05,0.05,0.05,0.10};
  // OptimalBst(p,q,5);
  // // cout << "here" << endl;
  // OptimalBstBetter(p,q,5);

  // char str[] = "aibohphobia";
  // // char str[] = "ivi";
  // LPS(str,strlen(str));

  // // vector<Point> points {{1,1},{2,7},{3,4},{6,3},{7,6},{8,2},{9,5}};
  // vector<Point> points {{0,6},{1,0},{2,3},{5,4},{6,1},{7,5},{8,2}};
  // BitonicTours(points);

  vector<string> words = {"abc", "def",   "gh",    "polq",
                          "cs",  "opaqe", "klfgh", "t"};
  // vector<string> words={"abc","deg"};
  PrintNeatly(words, 8);
}
