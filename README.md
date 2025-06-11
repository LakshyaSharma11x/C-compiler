🛠️ Mini C Compiler in C++
A simple compiler for a subset of the C language written in C++. It performs lexical analysis, parsing, and intermediate code generation (three-address code style).

🚀 Features
✅ Lexical analysis (tokenizer/lexer)

✅ Recursive descent parser

✅ Arithmetic expression parsing (+, -, *, /)

✅ Relational operators (<, >, ==, !=)

✅ Variable declarations and assignments

✅ return statements

✅ Control flow: if, while, {} blocks

✅ Intermediate code generation (3-address format)

🧠 Example Input
c
Copy
Edit
int main() {
    int x = 5;
    if (x < 10) {
        x = x + 1;
    }
    return x;
}
💡 Generated Output
text
Copy
Edit
x = 5
t0 = x < 10
IFZ t0 GOTO L0
t1 = x + 1
x = t1
GOTO L1
L0:
L1:
RET x
Compilation finished.
📁 Project Structure
bash
Copy
Edit
.
├── main.cpp          # Contains the Lexer, Parser, and main function
├── README.md         # This file
⚙️ How to Build & Run
🧱 Requirements
C++ compiler (g++, clang++)

C++11 or later

🔧 Compile
bash
Copy
Edit
g++ -std=c++11 main.cpp -o compiler
▶️ Run
bash
Copy
Edit
./compiler
Then paste your code (end with a closing } on its own line):

c
Copy
Edit
int main() {
    int a = 2;
    while (a < 5) {
        a = a + 1;
    }
    return a;
}
🔄 Roadmap
 Basic expression parsing

 Control structures (if, while)

 else block support

 for loops

 Error handling improvements

 Code optimization passes

 Real assembly generation (x86 / LLVM IR)

📚 Learning Goals
This project was built to understand:

Compiler design basics

Lexical analysis and recursive descent parsing

Generating intermediate representations (IR)

📜 License
MIT License — use it freely!

🙌 Acknowledgements
Inspired by classic compiler books and resources like:

Crafting Interpreters

Compilers: Principles, Techniques & Tools
