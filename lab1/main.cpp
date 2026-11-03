#include "Stack.h"
#include <sstream>
#include <vector>

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


template <typename T>
void demonstrate(const std::string& type_name,
                 std::size_t count,
                 const T& lower,
                 const T& upper) {
    std::cout << "=== Demonstrating Stack<" << type_name << "> ===\n";

    Stack<T> empty_stack;
    std::cout << "Default constructor: " << empty_stack << "\n";
    std::cout << "  empty() = " << (empty_stack.empty() ? "true" : "false")
              << ", size() = " << empty_stack.size() << "\n";

    Stack<T> rand_stack(count, lower, upper);
    std::cout << "Random constructor (" << count << " elements): "
              << rand_stack << "\n";
    std::cout << "  empty() = " << (rand_stack.empty() ? "true" : "false")
              << ", size() = " << rand_stack.size() << "\n";

    Stack<T> copy_stack(rand_stack);
    std::cout << "Copy constructor:    " << copy_stack << "\n";

    std::cout << "rand_stack == copy_stack: "
              << (rand_stack == copy_stack ? "true" : "false") << "\n";

    T extra_value = lower;
    rand_stack += extra_value;
    std::cout << "After += " << extra_value << ": " << rand_stack << "\n";
    std::cout << "  top() = " << rand_stack.top() << "\n";

    rand_stack -= 0;
    std::cout << "After -= (pop): " << rand_stack << "\n";

    std::cout << "rand_stack != copy_stack: "
              << (rand_stack != copy_stack ? "true" : "false") << "\n";

    Stack<T> assigned_stack;
    assigned_stack = rand_stack;
    std::cout << "After assignment:  " << assigned_stack << "\n";

    Stack<T> empty_for_exception;
    try {
        empty_for_exception -= 0;
    } catch (const std::exception& e) {
        std::cout << "Exception on pop from empty: " << e.what() << "\n";
    }

    try {
        (void)empty_for_exception.top();
    } catch (const std::exception& e) {
        std::cout << "Exception on top of empty: " << e.what() << "\n";
    }

    std::cout << "\n";
}

int main() {
    demonstrate<int>("int", 5, 1, 100);
    demonstrate<float>("float", 5, 1.0f, 10.0f);
    demonstrate<double>("double", 5, 1.0, 10.0);
    demonstrate<char>("char", 5, 'a', 'z');
    demonstrate<std::string>("std::string", 5,
                             std::string("aaa"), std::string("zzz"));

    std::cout << "=== Task 3: Postfix expression evaluation ===\n";

    std::string expr1 = "3 4 + 2 *";
    std::cout << "Expression: \"" << expr1 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr1) << "\n";

    std::string expr2 = "5 1 2 + 4 * + 3 -";
    std::cout << "Expression: \"" << expr2 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr2) << "\n";

    std::string expr3 = "8 2 / 3 +";
    std::cout << "Expression: \"" << expr3 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr3) << "\n";

    std::string expr4 = "2 5 -";
    std::cout << "Expression: \"" << expr4 << "\"\n";
    std::cout << "  Result = " << evaluate_postfix(expr4) << "\n";

    return 0;
}
