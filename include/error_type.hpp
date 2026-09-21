#ifndef ERROR_TYPE_HPP
#define ERROR_TYPE_HPP

enum class ErrorType {
    Null,
    UnknownCommand,
    FilenameEmpty,
    FilenameInvalid,
    FilenameNotFound,
    SceneAlreadyExists,
    SceneNotFound,
    ReadingIssue
};

#endif