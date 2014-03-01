/*
    ! file encoding: Windows-1251


   ------------------------------------------

    Самый быстрый INI ридер/райтер ( mxINI )

   ------------------


    ЧТО ЭТО
        - Это инклуд-файл для PAWN скриптов мультиплеера GTA San Andreas (SA-MP)

    КАК ИСПОЛЬЗОВАТЬ
        - Поместить этот файл в папку include, которая лежит рядом с компилятором
        - В самом верху вашего скрипта добавить #include <mxINI.inc>

    АВТОР
         MX_Master

    ВЕРСИЯ
        0.5 от 20.09.2010


    ДОСТУПНЫЕ ИНСТРУМЕНТЫ
    более подробные описания даны перед каждой функцией ниже в коде

        ini_createFile ( "путь/к/файлу.ini", "Содержимое файла по умолчанию" )

        ini_openFile ( "путь/к/файлу.ini" )

        ini_closeFile ( ИД_открытого_файла )
    --
        ini_setString ( ИД_открытого_файла, "имя ключа", "текстовое значение" )

        ini_setInteger ( ИД_открытого_файла, "имя ключа", 123456 )

        ini_setFloat ( ИД_открытого_файла, "имя ключа", 3.1416 )
    --
        ini_getString ( ИД_открытого_файла, "имя ключа", returnValue )

        ini_getInteger ( ИД_открытого_файла, "имя ключа", returnValue )

        ini_getFloat ( ИД_открытого_файла, "имя ключа", returnValue )
    --
        ini_removeKey ( ИД_открытого_файла, "имя ключа" )

        ini_getErrorInfo ( Код_ошибки )


    ПРИМЕРЫ

        new iniFile = ini_createFile ( "test4268.ini" );

        if ( iniFile < 0 )
            iniFile = ini_openFile ( "test4268.ini" );

        if ( iniFile >= 0 )
        {
            new returnString[32], returnNumber, Float: returnFloat;

            ini_setString  ( iniFile, "ключ со строкой", "текстовое значение" );
            ini_setInteger ( iniFile, "ключ с числом",   123456 );
            ini_setFloat   ( iniFile, "ключ с дробью",   3.1416 );

            ini_removeKey  ( iniFile, "ключ с числом" );

            ini_getString  ( iniFile, "ключ со строкой", returnString );
            ini_getInteger ( iniFile, "ключ с числом",   returnNumber );
            ini_getFloat   ( iniFile, "ключ с дробью",   returnFloat );


            ini_closeFile  ( iniFile );

            printf ( "\n `ключ_со_строкой` = `%s`,\n `ключ_с_числом` = `%d`,\n `ключ_с_дробью` = `%f` \n",
                returnString, returnNumber, returnFloat );
        }
        else print( "\n Не удалось открыть INI файл \n" );
*/











const

    // настройки

    // РЕКОМЕНДУЕТСЯ ИЗМЕНИТЬ ПОД СЕБЯ
    INI_MAX_OPENED_FILES    = 2,        // максимум, открытых одновременно, файлов
    INI_MAX_FILE_SIZE       = 65536,    // байт,        макс. размер файла
    INI_MAX_FILENAME_SIZE   = 128,      // символов,    макс. размер пути к файлу
    INI_MAX_KEYS_IN_FILE    = 2048,     // максимум ключей в открытом файле

    // в ОЗУ будет выделено для временного хранения файлов примерно вот столько байт:
    // INI_MAX_OPENED_FILES * ( INI_MAX_FILE_SIZE + INI_MAX_FILENAME_SIZE*4 + INI_MAX_KEYS_IN_FILE*4 )


    // НЕЛЬЗЯ МЕНЯТЬ
    INI_INTEGER_SIZE        = 12,       // размер строки с целочисленным значением
    INI_FLOAT_SIZE          = 40,       // размер строки с дробным числовым значением

    INI_STRING_DELIMITER    = '\n',     // разделитель строк
    INI_DELIMITER           = '=',      // разделитель ключа и значения


    // коды ошибок, возвращаемые функциями

    // РЕКОМЕНДУЕТСЯ НЕ ИЗМЕНЯТЬ
    INI_OK                  =  0,       // функция успешно выполнена

        // проверять на ошибку можно так:
        // if ( возвращаемое_значение_функции < 0 ) ...

    INI_FILE_NOT_FOUND      = -1,       // файл не найден по указанному пути
    INI_FILE_ALREADY_EXIST  = -2,       // файл не найден по указанному пути
    INI_TOO_LARGE_FILE      = -3,       // размер файла превысил допустимый лимит
    INI_WRONG_PATH_SIZE     = -4,       // неправильный размер пути к файлу
    INI_READ_ERROR          = -5,       // ошибка чтения файла
    INI_WRITE_ERROR         = -6,       // ошибка при записи в файл
    INI_NO_FREE_SLOT        = -7,       // нет свободного слота для открытия файла
    INI_WRONG_SLOT          = -8,       // указан неверный слот открытого файла
    INI_KEY_NOT_FOUND       = -9,       // ключ в открытом файле не найден
    INI_WRONG_RETURN_SIZE   = -10,      // размер строки, в которую будет помещено значение ключа - указан неверно (<= 0)


    // другие вспомогательные константы

    // НЕЛЬЗЯ МЕНЯТЬ
    cellbytes = cellbits / charbits;    // кол-во байт в одной ячейке





// списки разных символов для оператора case в одной из функций

// РЕКОМЕНДУЕТСЯ НЕ ИЗМЕНЯТЬ
#define INI_SPACE_CHAR  ' ', '\t'                       // строковые пробельные символы
#define INI_KEY_STARTS  ' ', '\t', '\r', '\n', '\0'     // символы, перед началом ключа
#define INI_STRING_ENDS '\r', '\n', '\0'                // символы, завершающие значение
#define INI_NUMBER_ENDS ' ', '\t', '\r', '\n', '\0'     // символы, завершающие численное значение





static stock

    // временное хранилище открытых файлов

    _ini_nSlotUsed      [ INI_MAX_OPENED_FILES ],                               // флаг: занят ли слот
    _ini_nFileChanged   [ INI_MAX_OPENED_FILES ],                               // флаг: был ли изменен файл
    _ini_nFileBusy      [ INI_MAX_OPENED_FILES ],                               // флаг: изменяется ли в данный момент содержимое файла
    _ini_nFileSize      [ INI_MAX_OPENED_FILES ],                               // размер открытого файла
    _ini_nDelimPos      [ INI_MAX_OPENED_FILES ] [ INI_MAX_KEYS_IN_FILE ],      // список позиций INI_DELIMITER
    _ini_nKeysCount     [ INI_MAX_OPENED_FILES ],                               // кол-во ключей открытого файла
    _ini_szFilePath     [ INI_MAX_OPENED_FILES ] [ INI_MAX_FILENAME_SIZE ],     // путь к файлу
    _ini_szFileContent  [ INI_MAX_OPENED_FILES ] [ INI_MAX_FILE_SIZE char ];    // контент файла





















/*
    Создает и сразу открывает INI файл для чтения/записи.

    ПОДРОБНЕЕ
        Файл создается только в ОЗУ, и в него записывается строка szDefaultContent.
        В szDefaultContent могут быть и ключи, которые потом парсер также будет видеть.
        Только при закрытии файла, его содержимое будет записано на диск
        по указанному пути к файлу.

    ПАРАМЕТРЫ:
        szFilePath[]        путь к файлу
        szDefaultContent    контент файла по умолчанию, обычно, можно не указывать

    ВЕРНЕТ:
        код ошибки < 0 или ИД_открытого_файла
*/

stock ini_createFile ( szFilePath[], szDefaultContent[] = "" )
{
    //
    //  несколько блоков с проверками
    //

    // ------------
    new nFileNameSize = strlen( szFilePath ); // узнаем размер пути

    // если размер пути неправильный
    if ( nFileNameSize <= 0 || nFileNameSize >= INI_MAX_FILENAME_SIZE )
        return INI_WRONG_PATH_SIZE; // вернем код ошибки
    // ------------


    // ------------
    if ( fexist( szFilePath ) ) // если файл уже существует
        return INI_FILE_ALREADY_EXIST; // вернем код ошибки - файл уже существует

    for ( new slot = 0; slot < INI_MAX_OPENED_FILES; slot++ ) // перебор всех слотов ОЗУ
        if // если уже есть такой открытый файл
        (
                _ini_nSlotUsed[slot] != 0
            &&  strcmp( szFilePath, _ini_szFilePath[slot], false ) == 0
        )
            return INI_FILE_ALREADY_EXIST; // вернем код ошибки - файл уже существует
    // ------------



    // ------------
    new nFileSize = strlen( szDefaultContent ); // узнаем размер контентa файла по умолчанию

    // если размер неправильный
    if ( nFileSize < 0 || nFileSize >= INI_MAX_FILE_SIZE )
        return INI_TOO_LARGE_FILE; // вернем код ошибки
    // ------------




    //
    //  поиск свободного слота для записи в ОЗУ
    //

    for ( new slot = 0; slot < INI_MAX_OPENED_FILES; slot++ ) // перебор всех слотов ОЗУ
    {
        if ( _ini_nSlotUsed[slot] != 1 ) // если слот найден
        {
            // ------------
            _ini_nSlotUsed[slot]        = 1; // застолбим найденное место
            _ini_nFileChanged[slot]     = 0; // файл не был изменен
            _ini_nFileSize[slot]        = nFileSize; // скопируем размер файла в слот
            _ini_nKeysCount[slot]       = 0; // кол-во ключей выставим 0
            // ------------


            // ------------
            // скопируем в ОЗУ весь файл,
            // запомнив все позиции INI_DELIMITER и их кол-во
            for ( new i = 0;  i < nFileSize && i < INI_MAX_FILE_SIZE;  i++ )
            {
                if (
                        szDefaultContent[i] == INI_DELIMITER // если символ это INI_DELIMITER
                    &&  _ini_nKeysCount[slot] < INI_MAX_KEYS_IN_FILE // и лимит ключей еще не исчерпан
                ) {
                    _ini_nDelimPos[slot][ _ini_nKeysCount[slot] ] = i; // добавим еще одну позицию INI_DELIMITER
                    _ini_nKeysCount[slot]++; // увеличим кол-во найденных ключей
                }

                _ini_szFileContent[slot]{i} = szDefaultContent[i];
            }

            _ini_szFileContent[slot]{nFileSize} = 0; // символ конца строки для контента
            // ------------


            // ------------
            // скопируем в озу путь к файлу
            memcpy( _ini_szFilePath[slot], szFilePath, 0, nFileNameSize * cellbytes );

            _ini_szFilePath[slot][nFileNameSize] = 0; // символ конца строки для пути
            // ------------


            return slot; // вернем ИД слота
        }
    }




    //
    //  если свободный слот в памяти не найден
    //

    return INI_NO_FREE_SLOT; // вернем код ошибки
    // ------------
}




/*
    Открывает INI файл для чтения/записи, если он существует.

    ПОДРОБНЕЕ
        Копирует в ОЗУ с диска всё содержимое файла, если его размер не превышает
        допустимый. Если кол-во ключей в файле больше допустимого, ошибки никакой не будет,
        просто при чтении/записи значений, парсер не будет видеть лишние ключи.

    ПАРАМЕТРЫ:
        szFilePath[]    путь к файлу

    ВЕРНЕТ:
        код ошибки < 0 или ИД_открытого_файла
*/

stock ini_openFile ( szFilePath[] )
{
    //
    //  несколько блоков с проверками
    //

    // ------------
    new nFileNameSize = strlen( szFilePath ); // узнаем размер пути

    // если размер пути неправильный
    if ( nFileNameSize <= 0 || nFileNameSize >= INI_MAX_FILENAME_SIZE )
        return INI_WRONG_PATH_SIZE; // вернем код ошибки
    // ------------


    if ( ! fexist( szFilePath ) ) // если файл не найден
        return INI_FILE_NOT_FOUND; // вернем код ошибки


    // ------------
    // проверка - открыт ли уже файл с таким именем
    for ( new slot = 0; slot < INI_MAX_OPENED_FILES; slot++ ) // перебор всех слотов ОЗУ
        if // если уже есть такой открытый файл
        (
                _ini_nSlotUsed[slot] != 0
            &&  strcmp( szFilePath, _ini_szFilePath[slot], false ) == 0
        )
            return slot; // просто вернем его слот
    // ------------


    // ------------
    new File: pFile = fopen( szFilePath, io_read ); // пытаемся открыть файл для чтения

    if ( ! pFile ) // если файл не открылся
        return INI_READ_ERROR; // вернем код ошибки
    // ------------


    // ------------
    new nFileSize = flength( pFile ); // узнаем размер файла

    if ( nFileSize >= INI_MAX_FILE_SIZE ) // если размер файла слишком большой
    {
        fclose(pFile); // закроем файл
        return INI_TOO_LARGE_FILE; // вернем код ошибки
    }
    // ------------




    //
    //  поиск свободного слота для записи в ОЗУ
    //

    // ------------
    for ( new slot = 0; slot < INI_MAX_OPENED_FILES; slot++ ) // перебор всех слотов ОЗУ
    {
        if ( _ini_nSlotUsed[slot] != 1 ) // если слот найден
        {
            // ------------
            _ini_nSlotUsed[slot]        = 1; // застолбим найденное место
            _ini_nFileChanged[slot]     = 0; // файл не был изменен
            _ini_nFileSize[slot]        = nFileSize; // скопируем размер файла в слот
            _ini_nKeysCount[slot]       = 0; // кол-во ключей выставим 0
            // ------------


            // ------------
            // скопируем в ОЗУ весь файл,
            // запомнив все позиции INI_DELIMITER и их кол-во
            for ( new i = 0, symbol; i < nFileSize && i < INI_MAX_FILE_SIZE; i++ )
            {
                symbol = fgetchar( pFile, 0, false ); // читаем из файла следующий символ

                if (
                        symbol == INI_DELIMITER // если символ это INI_DELIMITER
                    &&  _ini_nKeysCount[slot] < INI_MAX_KEYS_IN_FILE // и лимит ключей еще не исчерпан
                ) {
                    _ini_nDelimPos[slot][ _ini_nKeysCount[slot] ] = i; // добавим еще одну позицию INI_DELIMITER
                    _ini_nKeysCount[slot]++; // увеличим кол-во найденных ключей
                }

                _ini_szFileContent[slot]{i} = symbol;
            }

            _ini_szFileContent[slot]{nFileSize} = 0; // символ конца строки для контента

            fclose(pFile); // закроем файл
            // ------------


            // ------------
            // скопируем в озу путь к файлу
            memcpy( _ini_szFilePath[slot], szFilePath, 0, nFileNameSize * cellbytes );

            _ini_szFilePath[slot][nFileNameSize] = 0; // символ конца строки для пути
            // ------------


            return slot; // вернем ИД слота
        }
    }
    // ------------




    //
    //  если свободный слот в памяти не найден
    //

    // ------------
    fclose(pFile); // закроем файл

    return INI_NO_FREE_SLOT; // вернем код ошибки
    // ------------
}




/*
    Закрывает INI файл, если он был открыт.

    ПОДРОБНЕЕ
        Если файл не был изменен - освободит слот для хранения файла в ОЗУ.
        Если файл был изменен - полностью перезапишет файл на диске.

    ПАРАМЕТРЫ:
        nFilePointer    ИД_открытого_файла, полученный от ini_openFile / ini_createFile

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_closeFile ( nFilePointer )
{
    if
    (   // если ИД открытого файла указан верно
            nFilePointer >= 0
        &&  nFilePointer < INI_MAX_OPENED_FILES
        &&  _ini_nSlotUsed[nFilePointer] != 0
    )
    {
        if ( _ini_nFileChanged[nFilePointer] != 0 ) // если файл был изменен
        {
            new File: pFile = fopen( _ini_szFilePath[nFilePointer], io_write ); // пытаемся открыть файл для записи

            if ( ! pFile ) // если файл не открылся
                return INI_WRITE_ERROR; // вернем код ошибки

            // запишем контент файла из ОЗУ на диск
            for ( new i = 0;  i < _ini_nFileSize[nFilePointer];  i++ )
                fputchar( pFile, _ini_szFileContent[nFilePointer]{i}, false );

            fclose(pFile); // закроем файл
        }

        _ini_nSlotUsed[nFilePointer] = 0; // освободить слот открытого файла

        return INI_OK; // вернуть код об успешном выполнении функции
    }

    return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
}











/*
    Получает из открытого INI файла значение указанного ключа.

    ПОДРОБНЕЕ
        Парсер ищет в ОЗУ в контенте файла указанный ключ и помещает в
        szReturnValue его строковое значение. szReturnValue нужно создать заранее.

        nSizeOfReturn можно не указывать, если только не нужно точное кол-во
        возвращаемых символов в строке (в описании этого параметра ниже даны
        дополнительные указания).

        Как ключ так и значение в файле, могут быть любой длины и
        могут содержать любые символы, кроме 2 символов конца строки \r и \n.

        Парсер не видит пробелы и знаки табуляции вокруг имени ключа и
        перед значением, он считает их отступами.

    ПАРАМЕТРЫ:
        nFilePointer        ИД_открытого_файла, полученный от ini_openFile / ini_createFile
        szKeyName[]         имя ключа
        szReturnValue[]     сюда будет помещено значение ключа в виде строки
        nSizeOfReturn       макс. размер возвращаемой строки, обычно, это размер szReturnValue.
                            если ваша строка szReturnValue является частью массива, который был создан с помощью enum,
                            этот параметр нужно обязательно указывать как число

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_getString ( nFilePointer, szKeyName[], szReturnValue[], nSizeOfReturn = sizeof(szReturnValue) )
{
    // ----------------
    if // если ИД открытого файла указан неверно
    (
            nFilePointer < 0
        ||  nFilePointer >= INI_MAX_OPENED_FILES
        ||  _ini_nSlotUsed[nFilePointer] != 1
    )
        return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
    // ----------------


    if ( nSizeOfReturn <= 0 ) // если по какой-то причине размер возвращаемого значения указан/рассчитан как 0
        return INI_WRONG_RETURN_SIZE;


    // ----------------
    new nKeyLen = strlen(szKeyName); // узнаем длину имени указанного ключа

    if ( nKeyLen <= 0 ) // если указан пустой ключ
        return INI_KEY_NOT_FOUND;
    // ----------------


    // ----------------
    for // перебор и сравнение всех ключей файла с указанным ключом
    (
        new kPos = 0, curFilePos, found = 0;
        kPos < _ini_nKeysCount[nFilePointer];
        kPos++
    )
    {
        // ----------------
        found = 0; // флаг, найдена ли позиция конца ключа = 0

        for // ищем позицию конца ключа
        (
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] - 1; // текущ. поз. файла = поз. текущ. INI_DELIMITER - 1
            curFilePos >= 0; // продолжать пока поз. файла >= 0
            curFilePos-- // после каждого повтора текущ. поз. файла -= 1
        )
        {
            switch ( _ini_szFileContent[nFilePointer]{curFilePos} ) // узнаем что за символ в текущ. поз. файла
            {
                case INI_SPACE_CHAR :           continue; // если это пробельный символ, перейдем к пред. символу файла
                case INI_STRING_DELIMITER :     break; // если это конец строки
                default : // если это другой символ
                {
                    found = 1; // позиция конца ключа найдена
                    break; // конец цикла
                }
            }
        }

        // если позиция конца ключа не найдена, переход к след. позиции INI_DELIMITER
        if ( found != 1 ) continue;
        // ----------------


        // сравниваем посимвольно текущий ключ файла и указанный ключ
        for ( new curKeyPos = nKeyLen - 1;  curKeyPos >= 0;  curFilePos--, curKeyPos-- )
        {
            if
            (
                    curFilePos < 0 // если поз файла стала < 0
                ||  _ini_szFileContent[nFilePointer]{curFilePos} != szKeyName[curKeyPos] // если символы из ключей не равны
                ||  _ini_szFileContent[nFilePointer]{curFilePos} == INI_STRING_DELIMITER // если символ из ключа это INI_STRING_DELIMITER
            )
            {
                found = 0; // флаг, ключ не найден
                break; // конец сравнения
            }
        }


        // ----------------
        if ( found != 0 ) // если указанный ключ найден в файле
        {
            // если найдено совпадение не целого ключа файла, а его окончания с указанным ключом
            if ( curFilePos >= 0 )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_KEY_STARTS : {}
                    default: continue;
                }
            }


            // ----------------
            // текущая позиция в файле будет на 1 больше текущей позиции INI_DELIMITER
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] + 1;

            // ищем позицию начала значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_SPACE_CHAR :   continue; // если это пробельный символ, перейдем к след. символу
                    default :               break; // если это другой символ
                }
            }
            // ----------------


            // ----------------
            // скопируем посимвольно в szReturnValue значение ключа из файла
            // воспользуемся созданной переменной found как позицией в возвращаемом значении
            for ( found = 0;  found < nSizeOfReturn;  found++, curFilePos++ )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_STRING_ENDS : // если это символ конца значения
                    {
                        szReturnValue[found] = 0; // запишем символ конца строки
                        break; // конец копирования
                    }
                    default :
                        // копируем символ из файла в szReturnValue
                        szReturnValue[found] = _ini_szFileContent[nFilePointer]{curFilePos};
                }
            }

            szReturnValue[nSizeOfReturn - 1] = 0; // на всякий случай обрежем правильно строку

            return INI_OK;
            // ----------------
        }
        // ----------------
    }
    // ----------------


    // ----------------
    return INI_KEY_NOT_FOUND;
    // ----------------
}




/*
    Получает из открытого INI файла целочисленное значение указанного ключа.

    ПОДРОБНЕЕ
        Парсер ищет в ОЗУ в контенте файла указанный ключ и помещает в
        nReturnValue его целочисленное значение. nReturnValue нужно создать заранее.

        Имя ключа в файле может быть любой длины, а также может содержать
        любые символы, кроме 2 символов конца строки \r и \n.

        Значение может быть только фиксированной длины, которая равна INI_INTEGER_SIZE - 1.
        Парсер не видит пробелы и знаки табуляции вокруг имени ключа/значения,
        он считает их отступами.

    ПАРАМЕТРЫ:
        nFilePointer        ИД_открытого_файла, полученный от ini_openFile / ini_createFile
        szKeyName[]         имя ключа
        nReturnValue        сюда будет помещено значение ключа в виде целого числа

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_getInteger ( nFilePointer, szKeyName[], & nReturnValue )
{
    // ----------------
    if // если ИД открытого файла указан неверно
    (
            nFilePointer < 0
        ||  nFilePointer >= INI_MAX_OPENED_FILES
        ||  _ini_nSlotUsed[nFilePointer] != 1
    )
        return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
    // ----------------


    // ----------------
    new nKeyLen = strlen(szKeyName); // узнаем длину имени указанного ключа

    if ( nKeyLen <= 0 ) // если указан пустой ключ
        return INI_KEY_NOT_FOUND;
    // ----------------


    // ----------------
    for // перебор и сравнение всех ключей файла с указанным ключом
    (
        new kPos = 0, curFilePos, found = 0;
        kPos < _ini_nKeysCount[nFilePointer];
        kPos++
    )
    {
        // ----------------
        found = 0; // флаг, найдена ли позиция конца ключа = 0

        for // ищем позицию конца ключа
        (
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] - 1; // текущ. поз. файла = поз. текущ. INI_DELIMITER - 1
            curFilePos >= 0; // продолжать пока поз. файла >= 0
            curFilePos-- // после каждого повтора текущ. поз. файла -= 1
        )
        {
            switch ( _ini_szFileContent[nFilePointer]{curFilePos} ) // узнаем что за символ в текущ. поз. файла
            {
                case INI_SPACE_CHAR :           continue; // если это пробельный символ, перейдем к пред. символу файла
                case INI_STRING_DELIMITER :     break; // если это конец строки
                default : // если это другой символ
                {
                    found = 1; // позиция конца ключа найдена
                    break; // конец цикла
                }
            }
        }

        // если позиция конца ключа не найдена, переход к след. позиции INI_DELIMITER
        if ( found != 1 ) continue;
        // ----------------


        // сравниваем посимвольно текущий ключ файла и указанный ключ
        for ( new curKeyPos = nKeyLen - 1;  curKeyPos >= 0;  curFilePos--, curKeyPos-- )
        {
            if
            (
                    curFilePos < 0 // если поз файла стала < 0
                ||  _ini_szFileContent[nFilePointer]{curFilePos} != szKeyName[curKeyPos] // если символы из ключей не равны
                ||  _ini_szFileContent[nFilePointer]{curFilePos} == INI_STRING_DELIMITER // если символ из ключа это INI_STRING_DELIMITER
            )
            {
                found = 0; // флаг, ключ не найден
                break; // конец сравнения
            }
        }


        // ----------------
        if ( found != 0 ) // если указанный ключ найден в файле
        {
            // если найдено совпадение не целого ключа файла, а его окончания с указанным ключом
            if ( curFilePos >= 0 )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_KEY_STARTS : {}
                    default: continue;
                }
            }


            // ----------------
            // текущая позиция в файле будет на 1 больше текущей позиции INI_DELIMITER
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] + 1;

            // ищем позицию начала значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_SPACE_CHAR :   continue; // если это пробельный символ, перейдем к след. символу
                    default :               break; // если это другой символ
                }
            }
            // ----------------


            // ----------------
            new strValue[INI_INTEGER_SIZE]; // временная строка для численного значения

            // скопируем посимвольно в strValue значение ключа из файла
            // воспользуемся созданной переменной found как позицией в возвращаемом значении
            for ( found = 0;  found < INI_INTEGER_SIZE;  found++, curFilePos++ )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_NUMBER_ENDS : // если это символ конца численного значения
                    {
                        strValue[found] = 0; // запишем символ конца строки
                        break; // конец копирования
                    }
                    default :
                        // копируем символ из файла в strValue
                        strValue[found] = _ini_szFileContent[nFilePointer]{curFilePos};
                }
            }

            strValue[INI_INTEGER_SIZE - 1] = 0; // на всякий случай обрежем правильно строку

            nReturnValue = strval(strValue); // запишем в nReturnValue численное значение ключа

            return INI_OK;
            // ----------------
        }
        // ----------------
    }
    // ----------------


    // ----------------
    return INI_KEY_NOT_FOUND;
    // ----------------
}




/*
    Получает из открытого INI файла дробное численное значение указанного ключа.

    ПОДРОБНЕЕ
        Парсер ищет в ОЗУ в контенте файла указанный ключ и помещает в
        fReturnValue его дробное значение. fReturnValue нужно создать заранее.

        Имя ключа в файле может быть любой длины, а также может содержать
        любые символы, кроме 2 символов конца строки \r и \n.

        Значение может быть только фиксированной длины, которая равна INI_FLOAT_SIZE - 1.
        Парсер не видит пробелы и знаки табуляции вокруг имени ключа/значения,
        он считает их отступами.

    ПАРАМЕТРЫ:
        nFilePointer        ИД_открытого_файла, полученный от ini_openFile / ini_createFile
        szKeyName[]         имя ключа
        fReturnValue        сюда будет помещено значение ключа в виде дробного числа

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_getFloat ( nFilePointer, szKeyName[], & Float: fReturnValue )
{
    // ----------------
    if // если ИД открытого файла указан неверно
    (
            nFilePointer < 0
        ||  nFilePointer >= INI_MAX_OPENED_FILES
        ||  _ini_nSlotUsed[nFilePointer] != 1
    )
        return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
    // ----------------


    // ----------------
    new nKeyLen = strlen(szKeyName); // узнаем длину имени указанного ключа

    if ( nKeyLen <= 0 ) // если указан пустой ключ
        return INI_KEY_NOT_FOUND;
    // ----------------


    // ----------------
    for // перебор и сравнение всех ключей файла с указанным ключом
    (
        new kPos = 0, curFilePos, found = 0;
        kPos < _ini_nKeysCount[nFilePointer];
        kPos++
    )
    {
        // ----------------
        found = 0; // флаг, найдена ли позиция конца ключа = 0

        for // ищем позицию конца ключа
        (
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] - 1; // текущ. поз. файла = поз. текущ. INI_DELIMITER - 1
            curFilePos >= 0; // продолжать пока поз. файла >= 0
            curFilePos-- // после каждого повтора текущ. поз. файла -= 1
        )
        {
            switch ( _ini_szFileContent[nFilePointer]{curFilePos} ) // узнаем что за символ в текущ. поз. файла
            {
                case INI_SPACE_CHAR :           continue; // если это пробельный символ, перейдем к пред. символу файла
                case INI_STRING_DELIMITER :     break; // если это конец строки
                default : // если это другой символ
                {
                    found = 1; // позиция конца ключа найдена
                    break; // конец цикла
                }
            }
        }

        // если позиция конца ключа не найдена, переход к след. позиции INI_DELIMITER
        if ( found != 1 ) continue;
        // ----------------


        // сравниваем посимвольно текущий ключ файла и указанный ключ
        for ( new curKeyPos = nKeyLen - 1;  curKeyPos >= 0;  curFilePos--, curKeyPos-- )
        {
            if
            (
                    curFilePos < 0 // если поз файла стала < 0
                ||  _ini_szFileContent[nFilePointer]{curFilePos} != szKeyName[curKeyPos] // если символы из ключей не равны
                ||  _ini_szFileContent[nFilePointer]{curFilePos} == INI_STRING_DELIMITER // если символ из ключа это INI_STRING_DELIMITER
            )
            {
                found = 0; // флаг, ключ не найден
                break; // конец сравнения
            }
        }


        // ----------------
        if ( found != 0 ) // если указанный ключ найден в файле
        {
            // если найдено совпадение не целого ключа файла, а его окончания с указанным ключом
            if ( curFilePos >= 0 )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_KEY_STARTS : {}
                    default: continue;
                }
            }


            // ----------------
            // текущая позиция в файле будет на 1 больше текущей позиции INI_DELIMITER
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] + 1;

            // ищем позицию начала значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_SPACE_CHAR :   continue; // если это пробельный символ, перейдем к след. символу
                    default :               break; // если это другой символ
                }
            }
            // ----------------


            // ----------------
            new strValue[INI_FLOAT_SIZE]; // временная строка для дробного значения

            // скопируем посимвольно в strValue значение ключа из файла
            // воспользуемся созданной переменной found как позицией в возвращаемом значении
            for ( found = 0;  found < INI_INTEGER_SIZE;  found++, curFilePos++ )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_NUMBER_ENDS : // если это символ конца численного значения
                    {
                        strValue[found] = 0; // запишем символ конца строки
                        break; // конец копирования
                    }
                    default :
                        // копируем символ из файла в strValue
                        strValue[found] = _ini_szFileContent[nFilePointer]{curFilePos};
                }
            }

            strValue[INI_FLOAT_SIZE - 1] = 0; // на всякий случай обрежем правильно строку

            fReturnValue = floatstr(strValue); // запишем в fReturnValue дробное значение ключа

            return INI_OK;
            // ----------------
        }
        // ----------------
    }
    // ----------------


    // ----------------
    return INI_KEY_NOT_FOUND;
    // ----------------
}











/*
    Изменяет/добавляет в открытый INI файл указанный ключ и его значение.

    ПОДРОБНЕЕ
        Парсер ищет в ОЗУ в контенте файла указанный ключ и изменяет его
        строковое значение на szKeyValue.

        Форматирование файла не меняется. Если только указанное имя ключа
        не было найдено, тогда ключ и значение будут добавлены в конец файла.

        Имя ключа/значение в файле может быть любой длины, а также может содержать
        любые символы, кроме 2 символов конца строки \r и \n. Если эти символы есть
        в имени ключа или в значении, вы должны должны хорошо знать и осознавать
        последствия.

    ПАРАМЕТРЫ:
        nFilePointer        ИД_открытого_файла, полученный от ini_openFile / ini_createFile
        szKeyName[]         имя ключа
        szKeyValue[]        строка со значением ключа

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_setString ( nFilePointer, szKeyName[], szKeyValue[] )
{
    // ----------------
    if // если ИД открытого файла указан неверно
    (
            nFilePointer < 0
        ||  nFilePointer >= INI_MAX_OPENED_FILES
        ||  _ini_nSlotUsed[nFilePointer] != 1
    )
        return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
    // ----------------


    // ----------------
    new nKeyLen = strlen(szKeyName); // узнаем длину имени указанного ключа

    if ( nKeyLen <= 0 ) // если указан пустой ключ
        return INI_KEY_NOT_FOUND;
    // ----------------


    // ----------------
    for // перебор и сравнение всех ключей файла с указанным ключом
    (
        new kPos = 0, curFilePos, found;
        kPos < _ini_nKeysCount[nFilePointer];
        kPos++
    )
    {
        // ----------------
        found = 0; // флаг, найдена ли позиция конца ключа = 0

        for // ищем позицию конца ключа
        (
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] - 1; // текущ. поз. файла = поз. текущ. INI_DELIMITER - 1
            curFilePos >= 0; // продолжать пока поз. файла >= 0
            curFilePos-- // после каждого повтора текущ. поз. файла -= 1
        )
        {
            switch ( _ini_szFileContent[nFilePointer]{curFilePos} ) // узнаем что за символ в текущ. поз. файла
            {
                case INI_SPACE_CHAR :           continue; // если это пробельный символ, перейдем к пред. символу файла
                case INI_STRING_DELIMITER :     break; // если это конец строки
                default : // если это другой символ
                {
                    found = 1; // позиция конца ключа найдена
                    break; // конец цикла
                }
            }
        }

        // если позиция конца ключа не найдена, переход к след. позиции INI_DELIMITER
        if ( found != 1 ) continue;
        // ----------------


        // сравниваем посимвольно текущий ключ файла и указанный ключ
        for ( new curKeyPos = nKeyLen - 1;  curKeyPos >= 0;  curFilePos--, curKeyPos-- )
        {
            if
            (
                    curFilePos < 0 // если поз файла стала < 0
                ||  _ini_szFileContent[nFilePointer]{curFilePos} != szKeyName[curKeyPos] // если символы из ключей не равны
                ||  _ini_szFileContent[nFilePointer]{curFilePos} == INI_STRING_DELIMITER // если символ из ключа это INI_STRING_DELIMITER
            )
            {
                found = 0; // флаг, ключ не найден
                break; // конец сравнения
            }
        }


        if ( found != 0 ) // если указанный ключ НАЙДЕН в файле
        {
            // если найдено совпадение не целого ключа файла, а его окончания с указанным ключом
            if ( curFilePos >= 0 )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_KEY_STARTS : {}
                    default: continue;
                }
            }


            // ----------------
            // текущая позиция в файле будет на 1 больше текущей позиции INI_DELIMITER
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] + 1;

            // ищем позицию начала значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_SPACE_CHAR :   continue; // если это пробельный символ, перейдем к след. символу
                    default :               break; // если это другой символ
                }
            }
            // ----------------


            // ----------------
            new nValueLen = strlen(szKeyValue); // узнаем размер значения указанного ключа

            // если будущий размер файла превышает лимит
            if ( ( curFilePos + nValueLen ) >= INI_MAX_FILE_SIZE )
                return INI_TOO_LARGE_FILE; // вернем код ошибки о переполнении файла
            // ----------------


            // ----------------
            new fileValueStartPos = curFilePos; // сохраним позицию начала значения

            // ищем позицию конца значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_STRING_ENDS :  break; // если это конец строки - стоп
                    default :               continue; // если это другой символ, перейдем к след. символу
                }
            }

            // вычислим смещение оставшихся позиций для INI_DELIMITER после изменения значения
            new filePosOffset = nValueLen - ( /*текущ длина значения*/ curFilePos - fileValueStartPos );
            // ----------------


            if ( _ini_nFileBusy[nFilePointer] != 0 ) // если прямо сейчас файл изменяется другой функцией
                return INI_WRITE_ERROR; // вернем код ошибки при записи в файл


            // ----------------
            _ini_nFileBusy[nFilePointer] = 1; // флаг: файл изменяется = 1

            if ( filePosOffset != 0 ) // если длины старого и нового значений разные
            {
                if ( filePosOffset < 0 ) // если длина нового значения меньше
                {
                    for // копируем символы, стоящие после текущего значения на их новые места,
                    ( // которые теперь будут ближе к началу файла
                        ;
                        curFilePos < _ini_nFileSize[nFilePointer];
                        curFilePos++
                    )
                        _ini_szFileContent[nFilePointer]{ curFilePos + filePosOffset } =
                            _ini_szFileContent[nFilePointer]{curFilePos};
                }
                else if ( filePosOffset > 0 ) // если длина нового значения больше
                {
                    new fileValueEndPos = curFilePos;

                    for // копируем символы, стоящие после текущего значения на их новые места,
                    ( // начиная с конца файла
                        curFilePos = _ini_nFileSize[nFilePointer] - 1;
                        curFilePos >= fileValueEndPos;
                        curFilePos--
                    )
                        _ini_szFileContent[nFilePointer]{ curFilePos + filePosOffset } =
                            _ini_szFileContent[nFilePointer]{curFilePos};
                }

                // изменим позиции всех INI_DELIMITER, которые находились после текущего INI_DELIMITER
                for ( kPos++;  kPos < _ini_nKeysCount[nFilePointer];  kPos++ )
                    _ini_nDelimPos[nFilePointer][kPos] += filePosOffset;

                _ini_nFileSize[nFilePointer] += filePosOffset; // изменим размер файла
                _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] } = 0; // конец строки для конца файла
            }

            curFilePos = fileValueStartPos; // текущ поз файла = поз начала старого значения
            for // запишем новое значение поверх старого
            (
                new valuePos = 0;
                valuePos < nValueLen;
                curFilePos++, valuePos++
            )
                _ini_szFileContent[nFilePointer]{curFilePos} = szKeyValue[valuePos];

            _ini_nFileChanged[nFilePointer]     = 1; // флаг: файл изменен = 1
            _ini_nFileBusy[nFilePointer]        = 0; // флаг: файл изменяется = 0

            return INI_OK;
            // ----------------
        }
    }
    // ----------------


    //
    // если указанный ключ НЕ НАЙДЕН в файле
    //

    // ----------------
    new nValueLen = strlen(szKeyValue); // узнаем размер значения ключа

    // если будущий размер файла превышает лимит
    if ( ( _ini_nFileSize[nFilePointer] + 5 + nKeyLen + nValueLen ) >= INI_MAX_FILE_SIZE )
        return INI_TOO_LARGE_FILE; // вернем код ошибки о переполнении файла
    // ----------------


    // ----------------
    if ( _ini_nFileBusy[nFilePointer] != 0 ) // если прямо сейчас файл изменяется другой функцией
        return INI_WRITE_ERROR; // вернем код ошибки при записи в файл

    _ini_nFileBusy[nFilePointer] = 1; // флаг: файл изменяется = 1
    // ----------------


    // ----------------
    if // если последний символ файла не является разделителем строк
    (
            _ini_nFileSize[nFilePointer] > 0
        &&  _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] - 1 } != INI_STRING_DELIMITER
    )
    {
        // вставим в конец файла перевод каретки и разделитель строк
        _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] }        = '\r';
        _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] + 1 }    = INI_STRING_DELIMITER;

        _ini_nFileSize[nFilePointer] += 2; // увеличим размер файла на 2
    }

    new curFilePos, strPos; // будут временным хранилищами позиций в файле/ключе/значении

    for // добавим посимвольно имя ключа в файл
    (
        curFilePos = _ini_nFileSize[nFilePointer], strPos = 0;
        strPos < nKeyLen;
        curFilePos++, strPos++
    )
        _ini_szFileContent[nFilePointer]{curFilePos} = szKeyName[strPos];

    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = ' '; // после ключа добавим пробел

    _ini_nDelimPos[nFilePointer][ _ini_nKeysCount[nFilePointer] ]   = curFilePos; // добавим новую позицию INI_DELIMITER
    _ini_nKeysCount[nFilePointer]++; // кол-во ключей в файле +1

    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = INI_DELIMITER; // после пробела выше добавим INI_DELIMITER
    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = ' '; // после INI_DELIMITER добавим пробел

    // добавим посимвольно значение ключа в файл
    for ( strPos = 0;  strPos < nValueLen;  curFilePos++, strPos++ )
        _ini_szFileContent[nFilePointer]{curFilePos} = szKeyValue[strPos];

    _ini_szFileContent[nFilePointer]{curFilePos}    = 0; // добавим символ конца строки
    _ini_nFileSize[nFilePointer]                    = curFilePos; // изменим размер файла
    _ini_nFileChanged[nFilePointer]                 = 1; // флаг: файл изменен = 1
    _ini_nFileBusy[nFilePointer]                    = 0; // флаг: файл изменяется = 0

    return INI_OK;
    // ----------------
}




/*
    Изменяет/добавляет в открытый INI файл указанный ключ и его целочисленное значение.

    ПОДРОБНЕЕ
        Парсер ищет в ОЗУ в контенте файла указанный ключ и изменяет его
        целочисленное значение на nKeyValue.

        Форматирование файла не меняется, если только указанное имя ключа
        не было найдено, тогда ключ и значение будут добавлены в конец файла.

        Имя ключа в файле может быть любой длины, а также может содержать
        любые символы, кроме 2 символов конца строки \r и \n. Если вы в качестве
        значения указываете число больше, чем cellmax или меньше, чем cellmin,
        вы должны должны хорошо знать и осознавать последствия.

    ПАРАМЕТРЫ:
        nFilePointer        ИД_открытого_файла, полученный от ini_openFile / ini_createFile
        szKeyName[]         имя ключа
        nKeyValue           целочисленное значение ключа

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_setInteger ( nFilePointer, szKeyName[], nKeyValue )
{
    // ----------------
    if // если ИД открытого файла указан неверно
    (
            nFilePointer < 0
        ||  nFilePointer >= INI_MAX_OPENED_FILES
        ||  _ini_nSlotUsed[nFilePointer] != 1
    )
        return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
    // ----------------


    // ----------------
    new nKeyLen = strlen(szKeyName); // узнаем длину имени указанного ключа

    if ( nKeyLen <= 0 ) // если указан пустой ключ
        return INI_KEY_NOT_FOUND;
    // ----------------


    // ----------------
    for // перебор и сравнение всех ключей файла с указанным ключом
    (
        new kPos = 0, curFilePos, found;
        kPos < _ini_nKeysCount[nFilePointer];
        kPos++
    )
    {
        // ----------------
        found = 0; // флаг, найдена ли позиция конца ключа = 0

        for // ищем позицию конца ключа
        (
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] - 1; // текущ. поз. файла = поз. текущ. INI_DELIMITER - 1
            curFilePos >= 0; // продолжать пока поз. файла >= 0
            curFilePos-- // после каждого повтора текущ. поз. файла -= 1
        )
        {
            switch ( _ini_szFileContent[nFilePointer]{curFilePos} ) // узнаем что за символ в текущ. поз. файла
            {
                case INI_SPACE_CHAR :           continue; // если это пробельный символ, перейдем к пред. символу файла
                case INI_STRING_DELIMITER :     break; // если это конец строки
                default : // если это другой символ
                {
                    found = 1; // позиция конца ключа найдена
                    break; // конец цикла
                }
            }
        }

        // если позиция конца ключа не найдена, переход к след. позиции INI_DELIMITER
        if ( found != 1 ) continue;
        // ----------------


        // сравниваем посимвольно текущий ключ файла и указанный ключ
        for ( new curKeyPos = nKeyLen - 1;  curKeyPos >= 0;  curFilePos--, curKeyPos-- )
        {
            if
            (
                    curFilePos < 0 // если поз файла стала < 0
                ||  _ini_szFileContent[nFilePointer]{curFilePos} != szKeyName[curKeyPos] // если символы из ключей не равны
                ||  _ini_szFileContent[nFilePointer]{curFilePos} == INI_STRING_DELIMITER // если символ из ключа это INI_STRING_DELIMITER
            )
            {
                found = 0; // флаг, ключ не найден
                break; // конец сравнения
            }
        }


        if ( found != 0 ) // если указанный ключ НАЙДЕН в файле
        {
            // если найдено совпадение не целого ключа файла, а его окончания с указанным ключом
            if ( curFilePos >= 0 )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_KEY_STARTS : {}
                    default: continue;
                }
            }


            // ----------------
            // текущая позиция в файле будет на 1 больше текущей позиции INI_DELIMITER
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] + 1;

            // ищем позицию начала значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_SPACE_CHAR :   continue; // если это пробельный символ, перейдем к след. символу
                    default :               break; // если это другой символ
                }
            }
            // ----------------


            // ----------------
            new szKeyValue[INI_INTEGER_SIZE]; // создадим строку для хранения целочисленного значения
            format( szKeyValue, INI_INTEGER_SIZE, "%i", nKeyValue );
            // ----------------


            // ----------------
            new nValueLen = strlen(szKeyValue); // узнаем размер значения указанного ключа

            // если будущий размер файла превышает лимит
            if ( ( curFilePos + nValueLen ) >= INI_MAX_FILE_SIZE )
                return INI_TOO_LARGE_FILE; // вернем код ошибки о переполнении файла
            // ----------------


            // ----------------
            new fileValueStartPos = curFilePos; // сохраним позицию начала значения

            // ищем позицию конца значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_NUMBER_ENDS :  break; // если это конец числа - стоп
                    default :               continue; // если это другой символ, перейдем к след. символу
                }
            }

            // вычислим смещение оставшихся позиций для INI_DELIMITER после изменения значения
            new filePosOffset = nValueLen - ( /*текущ длина значения*/ curFilePos - fileValueStartPos );
            // ----------------


            if ( _ini_nFileBusy[nFilePointer] != 0 ) // если прямо сейчас файл изменяется другой функцией
                return INI_WRITE_ERROR; // вернем код ошибки при записи в файл


            // ----------------
            _ini_nFileBusy[nFilePointer] = 1; // флаг: файл изменяется = 1

            if ( filePosOffset != 0 ) // если длины старого и нового значений разные
            {
                if ( filePosOffset < 0 ) // если длина нового значения меньше
                {
                    for // копируем символы, стоящие после текущего значения на их новые места,
                    ( // которые теперь будут ближе к началу файла
                        ;
                        curFilePos < _ini_nFileSize[nFilePointer];
                        curFilePos++
                    )
                        _ini_szFileContent[nFilePointer]{ curFilePos + filePosOffset } =
                            _ini_szFileContent[nFilePointer]{curFilePos};
                }
                else if ( filePosOffset > 0 ) // если длина нового значения больше
                {
                    new fileValueEndPos = curFilePos;

                    for // копируем символы, стоящие после текущего значения на их новые места,
                    ( // начиная с конца файла
                        curFilePos = _ini_nFileSize[nFilePointer] - 1;
                        curFilePos >= fileValueEndPos;
                        curFilePos--
                    )
                        _ini_szFileContent[nFilePointer]{ curFilePos + filePosOffset } =
                            _ini_szFileContent[nFilePointer]{curFilePos};
                }

                // изменим позиции всех INI_DELIMITER, которые находились после текущего INI_DELIMITER
                for ( kPos++;  kPos < _ini_nKeysCount[nFilePointer];  kPos++ )
                    _ini_nDelimPos[nFilePointer][kPos] += filePosOffset;

                _ini_nFileSize[nFilePointer] += filePosOffset; // изменим размер файла
                _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] } = 0; // конец строки для конца файла
            }

            curFilePos = fileValueStartPos; // текущ поз файла = поз начала старого значения
            for // запишем новое значение поверх старого
            (
                new valuePos = 0;
                valuePos < nValueLen;
                curFilePos++, valuePos++
            )
                _ini_szFileContent[nFilePointer]{curFilePos} = szKeyValue[valuePos];

            _ini_nFileChanged[nFilePointer]     = 1; // флаг: файл изменен = 1
            _ini_nFileBusy[nFilePointer]        = 0; // флаг: файл изменяется = 0

            return INI_OK;
            // ----------------
        }
    }
    // ----------------


    //
    // если указанный ключ НЕ НАЙДЕН в файле
    //

    // ----------------
    new szKeyValue[INI_INTEGER_SIZE]; // создадим строку для хранения целочисленного значения
    format( szKeyValue, INI_INTEGER_SIZE, "%i", nKeyValue );
    // ----------------


    // ----------------
    new nValueLen = strlen(szKeyValue); // узнаем размер значения ключа

    // если будущий размер файла превышает лимит
    if ( ( _ini_nFileSize[nFilePointer] + 5 + nKeyLen + nValueLen ) >= INI_MAX_FILE_SIZE )
        return INI_TOO_LARGE_FILE; // вернем код ошибки о переполнении файла
    // ----------------


    // ----------------
    if ( _ini_nFileBusy[nFilePointer] != 0 ) // если прямо сейчас файл изменяется другой функцией
        return INI_WRITE_ERROR; // вернем код ошибки при записи в файл

    _ini_nFileBusy[nFilePointer] = 1; // флаг: файл изменяется = 1
    // ----------------


    // ----------------
    if // если последний символ файла не является разделителем строк
    (
            _ini_nFileSize[nFilePointer] > 0
        &&  _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] - 1 } != INI_STRING_DELIMITER
    )
    {
        // вставим в конец файла перевод каретки и разделитель строк
        _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] }        = '\r';
        _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] + 1 }    = INI_STRING_DELIMITER;

        _ini_nFileSize[nFilePointer] += 2; // увеличим размер файла на 2
    }

    new curFilePos, strPos; // будут временным хранилищами позиций в файле/ключе/значении

    for // добавим посимвольно имя ключа в файл
    (
        curFilePos = _ini_nFileSize[nFilePointer], strPos = 0;
        strPos < nKeyLen;
        curFilePos++, strPos++
    )
        _ini_szFileContent[nFilePointer]{curFilePos} = szKeyName[strPos];

    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = ' '; // после ключа добавим пробел

    _ini_nDelimPos[nFilePointer][ _ini_nKeysCount[nFilePointer] ]   = curFilePos; // добавим новую позицию INI_DELIMITER
    _ini_nKeysCount[nFilePointer]++; // кол-во ключей в файле +1

    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = INI_DELIMITER; // после пробела выше добавим INI_DELIMITER
    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = ' '; // после INI_DELIMITER добавим пробел

    // добавим посимвольно значение ключа в файл
    for ( strPos = 0;  strPos < nValueLen;  curFilePos++, strPos++ )
        _ini_szFileContent[nFilePointer]{curFilePos} = szKeyValue[strPos];

    _ini_szFileContent[nFilePointer]{curFilePos}    = 0; // добавим символ конца строки
    _ini_nFileSize[nFilePointer]                    = curFilePos; // изменим размер файла
    _ini_nFileChanged[nFilePointer]                 = 1; // флаг: файл изменен = 1
    _ini_nFileBusy[nFilePointer]                    = 0; // флаг: файл изменяется = 0

    return INI_OK;
    // ----------------
}




/*
    Изменяет/добавляет в открытый INI файл указанный ключ и его дробное численное значение.

    ПОДРОБНЕЕ
        Парсер ищет в ОЗУ в контенте файла указанный ключ и изменяет его
        дробное значение на fKeyValue.

        Форматирование файла не меняется, если только указанное имя ключа
        не было найдено, тогда ключ и значение будут добавлены в конец файла.

        Имя ключа в файле может быть любой длины, а также может содержать
        любые символы, кроме 2 символов конца строки \r и \n. Если вы в качестве
        значения указываете очень большое/маленькое дробное число, вы должны
        хорошо знать и осознавать последствия.

    ПАРАМЕТРЫ:
        nFilePointer        ИД_открытого_файла, полученный от ini_openFile / ini_createFile
        szKeyName[]         имя ключа
        fKeyValue           дробное численное значение ключа

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_setFloat ( nFilePointer, szKeyName[], Float: fKeyValue )
{
    // ----------------
    if // если ИД открытого файла указан неверно
    (
            nFilePointer < 0
        ||  nFilePointer >= INI_MAX_OPENED_FILES
        ||  _ini_nSlotUsed[nFilePointer] != 1
    )
        return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
    // ----------------


    // ----------------
    new nKeyLen = strlen(szKeyName); // узнаем длину имени указанного ключа

    if ( nKeyLen <= 0 ) // если указан пустой ключ
        return INI_KEY_NOT_FOUND;
    // ----------------


    // ----------------
    for // перебор и сравнение всех ключей файла с указанным ключом
    (
        new kPos = 0, curFilePos, found;
        kPos < _ini_nKeysCount[nFilePointer];
        kPos++
    )
    {
        // ----------------
        found = 0; // флаг, найдена ли позиция конца ключа = 0

        for // ищем позицию конца ключа
        (
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] - 1; // текущ. поз. файла = поз. текущ. INI_DELIMITER - 1
            curFilePos >= 0; // продолжать пока поз. файла >= 0
            curFilePos-- // после каждого повтора текущ. поз. файла -= 1
        )
        {
            switch ( _ini_szFileContent[nFilePointer]{curFilePos} ) // узнаем что за символ в текущ. поз. файла
            {
                case INI_SPACE_CHAR :           continue; // если это пробельный символ, перейдем к пред. символу файла
                case INI_STRING_DELIMITER :     break; // если это конец строки
                default : // если это другой символ
                {
                    found = 1; // позиция конца ключа найдена
                    break; // конец цикла
                }
            }
        }

        // если позиция конца ключа не найдена, переход к след. позиции INI_DELIMITER
        if ( found != 1 ) continue;
        // ----------------


        // сравниваем посимвольно текущий ключ файла и указанный ключ
        for ( new curKeyPos = nKeyLen - 1;  curKeyPos >= 0;  curFilePos--, curKeyPos-- )
        {
            if
            (
                    curFilePos < 0 // если поз файла стала < 0
                ||  _ini_szFileContent[nFilePointer]{curFilePos} != szKeyName[curKeyPos] // если символы из ключей не равны
                ||  _ini_szFileContent[nFilePointer]{curFilePos} == INI_STRING_DELIMITER // если символ из ключа это INI_STRING_DELIMITER
            )
            {
                found = 0; // флаг, ключ не найден
                break; // конец сравнения
            }
        }


        if ( found != 0 ) // если указанный ключ НАЙДЕН в файле
        {
            // если найдено совпадение не целого ключа файла, а его окончания с указанным ключом
            if ( curFilePos >= 0 )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_KEY_STARTS : {}
                    default: continue;
                }
            }


            // ----------------
            // текущая позиция в файле будет на 1 больше текущей позиции INI_DELIMITER
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] + 1;

            // ищем позицию начала значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_SPACE_CHAR :   continue; // если это пробельный символ, перейдем к след. символу
                    default :               break; // если это другой символ
                }
            }
            // ----------------


            // ----------------
            new szKeyValue[INI_FLOAT_SIZE]; // создадим строку для хранения дробного значения
            format( szKeyValue, INI_FLOAT_SIZE, "%f", fKeyValue ); // поместим строковое значение fKeyValue в szKeyValue
            // ----------------


            // ----------------
            new nValueLen = strlen(szKeyValue); // узнаем размер значения указанного ключа

            // если будущий размер файла превышает лимит
            if ( ( curFilePos + nValueLen ) >= INI_MAX_FILE_SIZE )
                return INI_TOO_LARGE_FILE; // вернем код ошибки о переполнении файла
            // ----------------


            // ----------------
            new fileValueStartPos = curFilePos; // сохраним позицию начала значения

            // ищем позицию конца значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_NUMBER_ENDS :  break; // если это конец числа - стоп
                    default :               continue; // если это другой символ, перейдем к след. символу
                }
            }

            // вычислим смещение оставшихся позиций для INI_DELIMITER после изменения значения
            new filePosOffset = nValueLen - ( /*текущ длина значения*/ curFilePos - fileValueStartPos );
            // ----------------


            if ( _ini_nFileBusy[nFilePointer] != 0 ) // если прямо сейчас файл изменяется другой функцией
                return INI_WRITE_ERROR; // вернем код ошибки при записи в файл


            // ----------------
            _ini_nFileBusy[nFilePointer] = 1; // флаг: файл изменяется = 1

            if ( filePosOffset != 0 ) // если длины старого и нового значений разные
            {
                if ( filePosOffset < 0 ) // если длина нового значения меньше
                {
                    for // копируем символы, стоящие после текущего значения на их новые места,
                    ( // которые теперь будут ближе к началу файла
                        ;
                        curFilePos < _ini_nFileSize[nFilePointer];
                        curFilePos++
                    )
                        _ini_szFileContent[nFilePointer]{ curFilePos + filePosOffset } =
                            _ini_szFileContent[nFilePointer]{curFilePos};
                }
                else if ( filePosOffset > 0 ) // если длина нового значения больше
                {
                    new fileValueEndPos = curFilePos;

                    for // копируем символы, стоящие после текущего значения на их новые места,
                    ( // начиная с конца файла
                        curFilePos = _ini_nFileSize[nFilePointer] - 1;
                        curFilePos >= fileValueEndPos;
                        curFilePos--
                    )
                        _ini_szFileContent[nFilePointer]{ curFilePos + filePosOffset } =
                            _ini_szFileContent[nFilePointer]{curFilePos};
                }

                // изменим позиции всех INI_DELIMITER, которые находились после текущего INI_DELIMITER
                for ( kPos++;  kPos < _ini_nKeysCount[nFilePointer];  kPos++ )
                    _ini_nDelimPos[nFilePointer][kPos] += filePosOffset;

                _ini_nFileSize[nFilePointer] += filePosOffset; // изменим размер файла
                _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] } = 0; // конец строки для конца файла
            }

            curFilePos = fileValueStartPos; // текущ поз файла = поз начала старого значения
            for // запишем новое значение поверх старого
            (
                new valuePos = 0;
                valuePos < nValueLen;
                curFilePos++, valuePos++
            )
                _ini_szFileContent[nFilePointer]{curFilePos} = szKeyValue[valuePos];

            _ini_nFileChanged[nFilePointer]     = 1; // флаг: файл изменен = 1
            _ini_nFileBusy[nFilePointer]        = 0; // флаг: файл изменяется = 0

            return INI_OK;
            // ----------------
        }
    }
    // ----------------


    //
    // если указанный ключ НЕ НАЙДЕН в файле
    //

    // ----------------
    new szKeyValue[INI_FLOAT_SIZE]; // создадим строку для хранения дробного значения
    format( szKeyValue, INI_FLOAT_SIZE, "%f", fKeyValue ); // поместим строковое значение fKeyValue в szKeyValue
    // ----------------


    // ----------------
    new nValueLen = strlen(szKeyValue); // узнаем размер значения ключа

    // если будущий размер файла превышает лимит
    if ( ( _ini_nFileSize[nFilePointer] + 5 + nKeyLen + nValueLen ) >= INI_MAX_FILE_SIZE )
        return INI_TOO_LARGE_FILE; // вернем код ошибки о переполнении файла
    // ----------------


    // ----------------
    if ( _ini_nFileBusy[nFilePointer] != 0 ) // если прямо сейчас файл изменяется другой функцией
        return INI_WRITE_ERROR; // вернем код ошибки при записи в файл

    _ini_nFileBusy[nFilePointer] = 1; // флаг: файл изменяется = 1
    // ----------------


    // ----------------
    if // если последний символ файла не является разделителем строк
    (
            _ini_nFileSize[nFilePointer] > 0
        &&  _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] - 1 } != INI_STRING_DELIMITER
    )
    {
        // вставим в конец файла перевод каретки и разделитель строк
        _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] }        = '\r';
        _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] + 1 }    = INI_STRING_DELIMITER;

        _ini_nFileSize[nFilePointer] += 2; // увеличим размер файла на 2
    }

    new curFilePos, strPos; // будут временным хранилищами позиций в файле/ключе/значении

    for // добавим посимвольно имя ключа в файл
    (
        curFilePos = _ini_nFileSize[nFilePointer], strPos = 0;
        strPos < nKeyLen;
        curFilePos++, strPos++
    )
        _ini_szFileContent[nFilePointer]{curFilePos} = szKeyName[strPos];

    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = ' '; // после ключа добавим пробел

    _ini_nDelimPos[nFilePointer][ _ini_nKeysCount[nFilePointer] ]   = curFilePos; // добавим новую позицию INI_DELIMITER
    _ini_nKeysCount[nFilePointer]++; // кол-во ключей в файле +1

    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = INI_DELIMITER; // после пробела выше добавим INI_DELIMITER
    _ini_szFileContent[nFilePointer]{ curFilePos++ }                = ' '; // после INI_DELIMITER добавим пробел

    // добавим посимвольно значение ключа в файл
    for ( strPos = 0;  strPos < nValueLen;  curFilePos++, strPos++ )
        _ini_szFileContent[nFilePointer]{curFilePos} = szKeyValue[strPos];

    _ini_szFileContent[nFilePointer]{curFilePos}    = 0; // добавим символ конца строки
    _ini_nFileSize[nFilePointer]                    = curFilePos; // изменим размер файла
    _ini_nFileChanged[nFilePointer]                 = 1; // флаг: файл изменен = 1
    _ini_nFileBusy[nFilePointer]                    = 0; // флаг: файл изменяется = 0

    return INI_OK;
    // ----------------
}




/*
    Удаляет из открытого INI файла указанный ключ и его значение.

    ПОДРОБНЕЕ
        Парсер ищет в ОЗУ в контенте файла указанный ключ и удаляет найденную
        пару ключ/значение.

        Имя ключа/значение в файле может быть любой длины,
        а также может содержать любые символы, кроме 2 символов конца строки \r и \n.

        Если эти символы есть в имени ключа или в значении,
        вы должны должны хорошо знать и осознавать последствия.

    ПАРАМЕТРЫ:
        nFilePointer        ИД_открытого_файла, полученный от ini_openFile / ini_createFile
        szKeyName[]         имя ключа

    ВЕРНЕТ:
        код ошибки < 0 или 0 при успехе
*/

stock ini_removeKey ( nFilePointer, szKeyName[] )
{
    // ----------------
    if // если ИД открытого файла указан неверно
    (
            nFilePointer < 0
        ||  nFilePointer >= INI_MAX_OPENED_FILES
        ||  _ini_nSlotUsed[nFilePointer] != 1
    )
        return INI_WRONG_SLOT; // вернуть код: неверный указатель на открытый файл
    // ----------------


    // ----------------
    new nKeyLen = strlen(szKeyName); // узнаем длину имени указанного ключа

    if ( nKeyLen <= 0 ) // если указан пустой ключ
        return INI_KEY_NOT_FOUND;
    // ----------------


    // ----------------
    for // перебор и сравнение всех ключей файла с указанным ключом
    (
        new kPos = 0, curFilePos, found;
        kPos < _ini_nKeysCount[nFilePointer];
        kPos++
    )
    {
        // ----------------
        found = 0; // флаг, найдена ли позиция конца ключа = 0

        for // ищем позицию конца ключа
        (
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] - 1; // текущ. поз. файла = поз. текущ. INI_DELIMITER - 1
            curFilePos >= 0; // продолжать пока поз. файла >= 0
            curFilePos-- // после каждого повтора текущ. поз. файла -= 1
        )
        {
            switch ( _ini_szFileContent[nFilePointer]{curFilePos} ) // узнаем что за символ в текущ. поз. файла
            {
                case INI_SPACE_CHAR :           continue; // если это пробельный символ, перейдем к пред. символу файла
                case INI_STRING_DELIMITER :     break; // если это конец строки
                default : // если это другой символ
                {
                    found = 1; // позиция конца ключа найдена
                    break; // конец цикла
                }
            }
        }

        // если позиция конца ключа не найдена, переход к след. позиции INI_DELIMITER
        if ( found != 1 ) continue;
        // ----------------


        // сравниваем посимвольно текущий ключ файла и указанный ключ
        for ( new curKeyPos = nKeyLen - 1;  curKeyPos >= 0;  curFilePos--, curKeyPos-- )
        {
            if
            (
                    curFilePos < 0 // если поз файла стала < 0
                ||  _ini_szFileContent[nFilePointer]{curFilePos} != szKeyName[curKeyPos] // если символы из ключей не равны
                ||  _ini_szFileContent[nFilePointer]{curFilePos} == INI_STRING_DELIMITER // если символ из ключа это INI_STRING_DELIMITER
            )
            {
                found = 0; // флаг, ключ не найден
                break; // конец сравнения
            }
        }


        if ( found != 0 ) // если указанный ключ НАЙДЕН в файле
        {
            // если найдено совпадение не целого ключа файла, а его окончания с указанным ключом
            if ( curFilePos >= 0 )
            {
                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_KEY_STARTS : {}
                    default: continue;
                }
            }


            // сохраним позицию начала ключа
            new keyStartPos = ( curFilePos <= 0 ) ? 0 : curFilePos + 1;


            // ----------------
            // текущая позиция в файле будет на 1 больше текущей позиции INI_DELIMITER
            curFilePos = _ini_nDelimPos[nFilePointer][kPos] + 1;

            // ищем позицию начала значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_SPACE_CHAR :   continue; // если это пробельный символ, перейдем к след. символу
                    default :               break; // если это другой символ
                }
            }
            // ----------------


            // ----------------
            // ищем позицию конца значения, она будет помещена в curFilePos
            for ( ; ; curFilePos++ )
            {
                if ( curFilePos >= _ini_nFileSize[nFilePointer] ) break;

                switch ( _ini_szFileContent[nFilePointer]{curFilePos} )
                {
                    case INI_STRING_ENDS :  break; // если это конец строки - стоп
                    default :               continue; // если это другой символ, перейдем к след. символу
                }
            }

            // вычислим смещение оставшихся позиций для INI_DELIMITER после изменения значения
            new filePosOffset = keyStartPos - curFilePos;
            // ----------------


            if ( _ini_nFileBusy[nFilePointer] != 0 ) // если прямо сейчас файл изменяется другой функцией
                return INI_WRITE_ERROR; // вернем код ошибки при записи в файл


            // ----------------
            _ini_nFileBusy[nFilePointer] = 1; // флаг: файл изменяется = 1

            // затираем контент ключа, следующими за ним данными файла
            for ( ; curFilePos <= _ini_nFileSize[nFilePointer]; curFilePos++ )
                _ini_szFileContent[nFilePointer]{ curFilePos + filePosOffset } =
                    _ini_szFileContent[nFilePointer]{curFilePos};

            // затираем текущий INI_DELIMITER, и меняем позиции последующих INI_DELIMITER
            for ( ;  kPos < _ini_nKeysCount[nFilePointer];  kPos++ )
                _ini_nDelimPos[nFilePointer][kPos] = _ini_nDelimPos[nFilePointer][kPos + 1] + filePosOffset;

            _ini_nKeysCount[nFilePointer]--;
            _ini_nFileSize[nFilePointer] += filePosOffset; // изменим размер файла
            _ini_szFileContent[nFilePointer]{ _ini_nFileSize[nFilePointer] } = 0; // конец строки для конца файла

            _ini_nFileChanged[nFilePointer]     = 1; // флаг: файл изменен = 1
            _ini_nFileBusy[nFilePointer]        = 0; // флаг: файл изменяется = 0

            return INI_OK;
            // ----------------
        }
    }
    // ----------------


    //
    // если указанный ключ НЕ НАЙДЕН в файле
    //

    // ----------------
    return INI_KEY_NOT_FOUND;
    // ----------------
}




/*
    Возвращает строку с описанием указанного кода ошибки на русском языке.

    ПОДРОБНЕЕ
        Если какая-то функция вам вернула не 0 (всё в порядке), а отрицательное
        значение, значит, имела место ошибка. Если вы хотите вывести в любой лог или
        в диалог человеческое описание этой ошибки, то передайте этой функции код
        ошибки и она вам вернет описание этой ошибки на русском языке.

        Описание ошибок довольно длинные и превышают размер в 128 символов,
        поэтому выводить эти описания в чате не рекомендуется!

    ПАРАМЕТРЫ:
        nErrorCode      код ошибки, полученный от любой функции

    ВЕРНЕТ:
        строку с человеческим описанием ошибки, под указанным кодом

    ПРИМЕР:
        new ini = ini_openFile("несуществующий файл"); // пытаемся открыть несуществующий файл

        if ( ini < 0 ) print( ini_getErrorInfo(ini) );
        else
        {
            new number, result = ini_getInteger( ini, "несуществующий ключ", number ); // пробуем получить значение несуществующего ключа

            if ( result < 0 ) print( ini_getErrorInfo(result) );

            ini_closeFile(ini);
        }
*/

stock ini_getErrorInfo ( nErrorCode )
{
    new error_ms[200] = "Ошибок нет";

    if ( nErrorCode >= 0 ) return error_ms;

    switch ( nErrorCode )
    {
        case INI_FILE_NOT_FOUND :     error_ms = "INI файл, указанный в `ini_openFile`, не существует, возможно, указан неверный путь к нему";
        case INI_FILE_ALREADY_EXIST : error_ms = "INI файл, указанный в `ini_createFile`, уже существует, укажите другое имя/путь к файлу";
        case INI_TOO_LARGE_FILE :     error_ms = "Невозможно добавить новый ключ в INI файл, т.к. размер INI файла превысит допустимый лимит в " #INI_MAX_FILE_SIZE " символ(ов), или будет превышен лимит в " #INI_MAX_KEYS_IN_FILE " ключей";
        case INI_WRONG_PATH_SIZE :    error_ms = "Длина пути к INI файлу, указанного в `ini_openFile` / `ini_createFile`, превышает лимит в " #INI_MAX_FILENAME_SIZE " символ(ов)";
        case INI_READ_ERROR :         error_ms = "Ошибка при чтении INI файла с диска, возможно, файл занят другим процессом";
        case INI_WRITE_ERROR :        error_ms = "Ошибка при записи содержимого INI файла на диск, возможно, файл занят другим процессом";
        case INI_NO_FREE_SLOT :       error_ms = "Лимит в " #INI_MAX_OPENED_FILES " одновременно открытых INI файлов исчерпан, поэтому открыть/создать еще 1 файл нельзя, пока не закрыт хоть 1 из открытых файлов";
        case INI_WRONG_SLOT :         error_ms = "Указан неверный ID открытого INI файла, возможно, при открытии INI файла произошла ошибка, поэтому вместо ID файла вы получили код ошибки";
        case INI_KEY_NOT_FOUND :      error_ms = "Указанный ключ не найден в открытом INI файле";
        case INI_WRONG_RETURN_SIZE :  error_ms = "При попытке чтения строкового значения из INI файла, последний параметр `nSizeOfReturn` в `ini_getString` оказался <= 0, укажите вручную значение > 0 для него";

        default:                      error_ms = "Неизвестная ошибка";
    }

    return error_ms;
}