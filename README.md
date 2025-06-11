# 🛠️ Mini C Compiler in C++

A simple compiler for a subset of the C language written in C++. It performs **lexical analysis**, **parsing**, and **intermediate code generation** (three-address code style).

---

## 🚀 Features

- ✅ Lexical analysis (tokenizer/lexer)
- ✅ Recursive descent parser
- ✅ Arithmetic expression parsing (`+`, `-`, `*`, `/`)
- ✅ Relational operators (`<`, `>`, `==`, `!=`)
- ✅ Variable declarations and assignments
- ✅ `return` statements
- ✅ Control flow: `if`, `while`, `{}` blocks
- ✅ Intermediate code generation (3-address format)

---



## 📚 Learning Goals

This project was created to deepen my understanding of:

- 🔍 **Compiler Design** – Building a minimal compiler pipeline
- 🧱 **Lexical Analysis** – Tokenizing input code into meaningful units
- 🌳 **Recursive Descent Parsing** – Parsing C-like syntax without parser generators
- 🧠 **Intermediate Representation** – Translating code into 3-address format
- 🧪 **Experimentation** – Trying out new language features and logic building

## 🧠 Example Input

```c
int main() {
    int x = 5;
    if (x < 10) {
        x = x + 1;
    }
    return x;
}

---

