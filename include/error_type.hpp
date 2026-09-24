#ifndef ERROR_TYPE_HPP
#define ERROR_TYPE_HPP

// Enum class for returning and printing errors to console.
enum class ErrorType {
    Null, // Empty variant
    UnknownCommand, // User inputs anything not handled by the program
    FilenameEmpty, // Name of scene is empty
    FilenameInvalid, //  User inputs an invalid name for a file
    FilenameNotFound, // File could not be found
    SceneAlreadyExists, // User tries to name a new scene after one that already exists
    SceneNotFound, // User tries to load/render a scene that doesn't exist
    ReadingIssue, // An issue occurred with file i/o
    InvalidObjectName, // User is trying to modify/create/delete an object with an invalid type/name
    ObjectAlreadyExists // User tries to create an object with a name that is already being used
};

#endif