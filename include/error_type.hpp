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
    ObjectAlreadyExists, // User tries to create an object with a name that is already being used
    ObjectNameEmpty,
    ObjectNotFound, // Instance of an object could not be found by a method
    InvalidValue, // Value given to scene variable is invalid
    EmptyValue, // Empty value when attempting to change a object property
    InvalidSceneFile, // Scene was unable to be rendered
    InvalidColour, // Invalid colour value, must be in [0, 1]
    NegativeValue // Numeric value is less than 0
};

#endif