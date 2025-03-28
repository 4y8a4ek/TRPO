// #include <string>
// #include <iostream>

// using namespace std;

// string cut_zeros(string st) {
//     // Если точки нет, возвращаем строку без изменений
//     if (st.find('.') == string::npos) {
//         return st;
//     }

//     // Удаляем конечные нули, но оставляем саму точку, если она в конце
//     while (!st.empty() && st.back() == '0') {
//         st.pop_back();
//     }

//     // Если после удаления осталась точка в конце, тоже удаляем её
//     if (!st.empty() && st.back() == '.') {
//         st.pop_back();
//     }

//     return st;
// }

// int main() {
//     cout << cut_zeros("1000") << endl;       // "1000" (без изменений)
//     cout << cut_zeros("123.000") << endl;    // "123"
//     cout << cut_zeros("4500") << endl;       // "4500" (без изменений)
//     cout << cut_zeros("0") << endl;          // "0" (без изменений)
//     cout << cut_zeros("500.1000") << endl;   // "500.1"
//     cout << cut_zeros("1010.00") << endl;    // "1010"
//     cout << cut_zeros("0.5000") << endl;     // "0.5"
//     cout << cut_zeros("5.0") << endl;        // "5"
//     cout << cut_zeros("110") << endl;
// }
