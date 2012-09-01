#ifndef __art_struct_h_
#define __art_struct_h_

#include <string>
#include "art_defs.h"

using namespace std;

// type T; if(T oper value)
class CExpr{
public:
    string	oper,	// операция: "==", "!=", ">", ...
    type,	// пользовательский тип: "int", "ssize_t", "gint", ...
    // type == "" если это XXX
    value;	// пользовательское значение: "0", "NULL", "-1", "ERR_OK", ...
    
#ifdef ART_DEBUG
    void dump(size_t ntabs);
#endif
};

// Note: vector <CArg> args; В args[0] поле .name всегда пусто;
// args[0] используется для указания типа возвращаемого значения.
class CArg{
public:
    string	type,	// пользовательский тип: "int", "void *", "LTGERR", "MyType", ...
    name;	// пользовательское имя: "fd", "b", "ltgerr", "MyValidValue", ...
    
#ifdef ART_DEBUG
    void dump(size_t ntabs);
#endif
};

#include <vector>

// *Tor's class
class CTor{
public:
    string	name;		// название Tor'a: "malloc", "fopen", ...

    vector	<CArg> args;	// args[0] = return-type, args[1] = первый аргумент, ...

    /* Аргументы считаются с нуля */

    size_t	float_arg_n1;	// номер первого аргумента, в котором значение delta
    // или len.

    size_t	float_arg_n2;	// номер второго аргумента, в котором значение delta
    // или len.
    // 0 - если его нет.
    // !float_arg_n1 --> !float_arg_n1
    // ???Если их два, то значения перемножаются???

    vector	<string> validateDom_before;	// 1 - 1st arg's domain name.
    // 0 - always "" (and ignorable by libgen)
    // проверять i-й аргумент по правилам домена
    // validateDom[i]: "stdmem", ...
    // ДО выполнения операции.
    // "" если проверять не надо

    bool    is_handle_arg_out; // есть возвращаемый handle или нет.
    // если есть, то учитывается handle_arg_out.
    // если нет, handle_arg_out игнорируется.
    // введено в версии 2009-12-02, т.к. MAX_VALID_HANDLE_ARG
    // более не используется.

    size_t	handle_arg_out;	// номер выходящего handl'а. Значение больше
    // MAX_VALID_HANDLE_ARG, если его нет.
    // По этому номеру выполняется проверка по правилам
    // текущего Домена.
    // Обычно используется для ALLOCATOR'а.
    // 0 - проверять фактически retval
    // i - проверять i-й аргумент
    // NOTE: когда значение равно 0, то в шаблоне я пишу
    // не проверять badRetCode посредством задания
    // .type = ""

    CExpr	badRetCode;	// .type = "void" если возвращаемого значения нет;
    // .type = "" если не нужно выполнять evalution,
    // например в случае когда возвращаемое значение
    // есть то же самое что и Handle.
    string	cpp_alias;	// Указывает что 1) это С++ тор
    // 2) С-Генераторы должны игнорировать этот тор на
    // стадии генерации declaration'ов, redefinition'ов;
    // А вот на стадии генерации ACTION'а использовать это
    // имя вместо вызова .name'а...(этот код вызывается
    // при работе С++-генератора)

#ifdef ART_DEBUG
    void dump(size_t ntabs);
#endif
};

class CDomain{
public:
    string	name;		// имя
    bool	float_handle;	// HANDLE как id(Атомарное) или как [x..y](Плавающее)
    string	handle;		// void*, int, FILE*, ...
    CExpr	bad_handle;
    bool    includesGlobal; // true -> #include <...>, else #include "..."
    vector	<string> includes;	// art.h includes it(to have prototypes)
    vector	<CTor>	allocators,
    deallocators,
    reallocators,
    operators;

    string  ccLabel; //метка, использующаяся в заголовочном файле с определением этого ресурса
                     //для организации условной компиляции
#ifdef ART_DEBUG
    void dump(size_t ntabs);
#endif
};

class CTemplate{
public:
    string	name;
    string	prefix;	// префикс к перехватываемым функциям: "art_", ...
    string	errlogmode;	// "" никуда не выводить,
    // "console" - использовать консоль(stderr)
    // "filename" - использовать файл
    bool	trap_on_io_err; // true -> i/o ошибки во время работы art.lib приводят
    // к arterrlog() + exit();
    // false -> игнорируются
    bool	remote_mode;	// true -> artlib будет использовать tcpip
    bool	force_flush;	// normaly must be enabled
    bool    fork_enabled;   // if false, then artlib tryes to detect
    // fork() and halt the application. if true (not yet implemented), then
    // allows handling programm with fork()
    bool	multithreaded;	// указывает art.lib'у является ли приложение
    // многопоточным
    bool    art_start_selfinit; // включить поддержку трассировки
    // без необходимости явного вызова art_start()
    // art_start() вызывается при вызове первой перехваченной функции
    string	threading;	// указывает art.lib'у тип многопоточного приложения
    // "posix", "win32"
    string	compiler_type;	// указывает art.lib'у тип компилятора
    // "gcc", "win32"
    string	trace_target; 	// if remote_mode -> `"host", port` || `"ip", port`
    // else `"tracefile"`
    vector	<CDomain> domains;
    vector	<CTor> specialTors;	// Tors with arguments from different Domains.
    // like fread(), read(), ...
    // vector <CComplexTor> complexTor;
    // Смотри глобальную цель 33

#ifdef ART_DEBUG
    void dump(void);
#endif
};

#endif
