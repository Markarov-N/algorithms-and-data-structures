#include "Stack.h"
#include <sstream>
#include <vector>

// Task 3: evaluate a postfix (RPN) expression with double operands.
// Supported operators: + - * /
double evaluate_postfix(const std::string& expression) {
    Stack<double> stack;
    std::istringstream stream(expression);
    std::string token;

    while (stream >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stack.size() < 2) {
                throw std::runtime_error(
                    "evaluate_postfix: not enough operands for operator '"
                    + token + "'");
            }

            double right = stack.top();
            stack -= 0;
            double left = stack.top();
            stack -= 0;

            double result = 0.0;
            if (token == "+")      result = left + right;
            else if (token == "-") result = left - right;
            else if (token == "*") result = left * right;
            else if (token == "/") {
                if (std::fabs(right) < Stack<double>::kEpsilon) {
                    throw std::runtime_error(
                        "evaluate_postfix: division by zero");
                }
                result = left / right;
            }
            stack += result;
        } else {
            // token is an operand
            stack += std::stod(token);
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error(
            "evaluate_postfix: invalid expression, "
            "stack size is " + std::to_string(stack.size()));
    }

    return stack.top();
}

// Demonstrate all operators and methods for a given type
template <typename T>
void demonstrate(const std::string& type_name,
                 std::size_t count,
                 const T& lower,
                 const T& upper) {
    std::cout << "=== Demonstrating Stack<" << type_name << "> ===\n";

    // Default constructor
    Stack<T> empty_stack;
    std::cout << "Default constructor: " << empty_stack << "\n";
    std::cout << "  empty() = " << (empty_stack.empty() ? "true" : "false")
              << ", size() = " << empty_stack.size() << "\n";

    // Random-value constructor
    Stack<T> rand_stack(count, lower, upper);
    std::cout << "Random constructor (" << count << " elements): "
              << rand_stack << "\n";
    std::cout << "  empty() = " << (rand_stack.empty() ? "true" : "false")
              << ", size() = " << rand_stack.size() << "\n";

    // Copy constructor
    Stack<T> copy_stack(rand_stack);
    std::cout << "Copy constructor:    " << copy_stack << "\n";

    // Equality
    std::cout << "rand_stack == copy_stack: "
              << (rand_stack == copy_stack ? "true" : "false") << "\n";

    // operator+= (push)
    T extra_value = lower;
    rand_stack += extra_value;
    std::cout << "After += " << extra_value << ": " << rand_stack << "\n";
    std::cout << "  top() = " << rand_stack.top() << "\n";

    // operator-= (pop)
    rand_stack -= 0;
    std::cout << "After -= (pop): " << rand_stack << "\n";

    // Inequality
    std::cout << "rand_stack != copy_stack: "
              << (rand_stack != copy_stack ? "true" : "false") << "\n";

    // Copy assignment
    Stack<T> assigned_stack;
    assigned_stack = rand_stack;
    std::cout << "After assignment:  " << assigned_stack << "\n";

    // Exception on pop from empty stack
    Stack<T> empty_for_exception;
    try {
        empty_for_exception -= 0;
    } catch (const std::exception& e) {
        std::cout << "Exception on pop from empty: " << e.what() << "\n";
    }

    // Exception on top of empty stack
    try {
        (void)empty_for_exception.top();
    } catch (const std::exception& e) {
        std::cout << "Exception on top of empty: " << e.what() << "\n";
    }

    std::cout << "\n";
}

int main() {
    // Demonstrate for all required types
    demonstrate<int>("int", 5, 1, 100);
    demonstrate<float>("float", 5, 1.0f, 10.0f);
    demonstrate<double>("double", 5, 1.0, 10.0);
    demonstrate<char>("char", 5, 'a', 'z');
    demonstrate<std::string>("std::string", 5,
                             std::string("aaa"), std::string("zzz"));

    // Task 3: evaluate postfix expression
    std::cout << "=== Task 3: Postfix expression evaluation ===\n";

    // Example: (3 + 4) * 2  =>  3 4 + 2 *
    std::string expr1 = "3 4 + 2 *";
    std::cout << "Expression: \"" << expr1 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr1) << "\n";
    // Expected: (3+4)*2 = 14

    // Example: 5 1 2 + 4 * + 3 -  =>  5 + (1+2)*4 - 3
    std::string expr2 = "5 1 2 + 4 * + 3 -";
    std::cout << "Expression: \"" << expr2 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr2) << "\n";
    // Expected: 5 + 12 - 3 = 14

    // Example with division: 8 2 / 3 +
    std::string expr3 = "8 2 / 3 +";
    std::cout << "Expression: \"" << expr3 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr3) << "\n";
    // Expected: 4 + 3 = 7

    // Example with negative result: 2 5 -
    std::string expr4 = "2 5 -";
    std::cout << "Expression: \"" << expr4 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr4) << "\n";
    // Expected: -3

    return 0;
}
