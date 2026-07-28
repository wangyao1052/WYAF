///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY_ERRORS_H
#define WY_ERRORS_H

namespace wy
{
    // 错误码
    enum class ErrorStatus
    {
        Ok = 0,

        Error               = 1,
        NotImplementedYet   = 2,
        NotSupported        = 3,
        NotCurrentlyAllowed = 4,
        NotAvailable        = 5,
        OutOfRange          = 6,
        SystemBusy          = 7,

        NullInputPointer       = 50,
        NullElementPointer     = 51,
        NullElementId          = 52,
        NullDatabasePointer    = 53,
        NullTransactionPointer = 54,
        NullDocumentPointer    = 55,
        NullEnvironmentPointer = 56,

        InvalidInput             = 100,
        InvalidElementId         = 101,
        InvalidParameterName     = 102,
        InvalidElementTableIndex = 103,
        InvalidFileName          = 104,

        WrongDatabase            = 150,
        ElementIdMismatch        = 151,

        KeyNotFound       = 200,
        DuplicateKey      = 201,
        InvalidIndex      = 202,
        DuplicateIndex    = 203,
        AlreadyInDatabase = 204,
        DuplicateFileName = 205,

        ParameterNotFound = 250,
        ParameterReadonly = 251,

        // element
        ElementNotFound                 = 300,
        ElementNotNewlyCreated          = 301,
        ElementNotOpenedForWrite        = 302,
        ElementErased                   = 303,
        ElementNotInTransaction         = 304,
        ElementNotInDatabase            = 305,
        NewElementNotAddedToTransaction = 306,

        NoActiveTransaction          = 400,
        NoFieldValue                 = 401,
        FieldNotFound                = 402,
        TransactionManagerBusy       = 403,
        TransactionGroupNotSupported = 404,
        TransactionLeafNotSupported  = 405,

        FilerError          = 500,
        CantOpenFile        = 501,
        DatabaseNotEmpty    = 502,
        BadFile             = 503,
        FileVersionTooNew   = 504,
        FilePathIsDirectory = 505,
        DirectoryNotFound   = 506,

        DocumentNotManaged    = 600,
        NoActiveDocument      = 601,
        ActiveDocumentChanged = 602,
        DocumentActive        = 603,
        DocumentNotFound      = 604,
        DocumentManagerBusy   = 605,

        CommandAlreadyExists              = 700,
        CommandNotFound                   = 701,
        CommandRejected                   = 702,
        CommandCanceled                   = 703,
        CommandStartFailed                = 704,
        CommandFailed                     = 705,
        ModalCommandInProgress            = 706,
        NoCurrentModalCommand             = 707,
        TransparentCommandMustBeImmediate = 708,
        InvalidCommandName                = 709,
        NotCommandExecutionClass          = 710,
        CommandManagerBusy                = 711,

        NoActiveEnvironment            = 800,
        ActiveEnvironmentChanged       = 801,
        InvalidEnvironmentTransition   = 802,
        NotAllowedInCurrentEnvironment = 803,
        EnvironmentManagerBusy         = 804,

        SelectionManagerBusy = 900,
        GizmoManagerBusy     = 950,

        InvalidFilePath       = 1000,
        FileSystemError       = 1001,
        FileNotFound          = 1002,
        NotRegularFile        = 1003,
        UnsupportedFileFormat = 1004,

        InitFreeTypeError         = 5000,
        FreeTypeLoadFontFaceError = 5001,
        FreeTypeDecodeError       = 5002,
    };
}

#endif // WY_ERRORS_H