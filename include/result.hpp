#include <variant>

// Primary result template.
template<typename T, typename E>
class Result {
    public:
    
        static Result Success(T value) {
            return Result(value);
        }

        static Result Failure(E error) {
            return Result(error);
        }

        // Checks if a variant contains a T.
        bool HasValue() const {
            return std::holds_alternative<T>(r_value);
        }

        // Returns the success value of the Result.
        T& Value() {
            return std::get<T>(r_value);
        }

        // Returns the failure value of the Result.
        const E& Error() {
            return std::get<E>(r_value);
        }

    private:
        std::variant<T, E> r_value;

        // Private constructors for code readability.
        Result(T value) { r_value = value; }

        Result(E error) { r_value = error; }

};

// Specialization for when there is no success return type.
template<typename E>
class Result<void, E> {
    public:
        static Result Success() {
            return Result();
        }

        static Result Failure(E error) {
            return Result(error);
        }

        // Checks if a variant contains a monostate.
        bool HasValue() const {
            return std::holds_alternative<std::monostate>(r_value);
        }

        // Returns the failure value of the Result.
        const E& Error() {
            return std::get<E>(r_value);
        }

    private:
        // Default r_value is std::monostate 
        std::variant<std::monostate, E> r_value;

        // Private constructors for code readability.
        // Result() {}

        Result(E error) { r_value = error; }

        
};
