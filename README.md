# Karnaugh-Map-
# Karnaugh-Map Minimizer

**Karnaugh Map (K-Map) Minimizer** built in **C++**, capable of simplifying Boolean expressions for up to **5 variables**. It uses the **Quine–McCluskey algorithm** to output the minimized expression in **Sum of Products (SOP)** form.

---

## Features

- Supports **2 to 5 input variables**
- Accepts **minterms** and **don't care terms**
- Generates minimized Boolean **SOP expression**
- Built entirely with **standard C++**
- No external libraries required

---

##  How to Run

1. **Clone** the repository or download the source code.
2. **Compile** the program using a C++ compiler:
   ```bash
   g++ -o kmap_minimizer kmap_minimizer.cpp
   ./kmap_minimizer
## Follow this prompts to enter 

1. Number of variables (between 2 and 5)
2. Minterms (space separated, end input with -1)
3. Don’t care terms (optional, end input with -1)

## Example: 

```bash
Enter number of variables (2 to 5): 4
Enter minterms (space separated), end with -1:
1 3 7 8 -1
Enter don't care terms (space separated), end with -1:
2 5 -1

Simplified SOP Expression:
A'B'C + AB + A'BC'


