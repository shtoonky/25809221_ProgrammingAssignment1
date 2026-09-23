#ifndef ERROR_TYPE_HPP
#define ERROR_TYPE_HPP

// Enum class for returning and printing errors to console.
enum class ErrorType {
    Null, // For when there is no error
    UnknownCommand,
    FilenameEmpty,
    FilenameInvalid,
    FilenameNotFound,
    SceneAlreadyExists,
    SceneNotFound,
    ReadingIssue
};

#endif