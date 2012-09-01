#include "gen_xml_sax_read.h"

#include <iostream>

artSaxParser::artSaxParser() :
    xmlpp::SaxParser() {
}

artSaxParser::~artSaxParser() {
}

void artSaxParser::on_start_document() {
    //std::cout << "on_start_document()" << std::endl;
    xml_metachunk chunk;

    chunk.event = onDocStart;

    chunks.push_back(chunk);
}

void artSaxParser::on_end_document() {
    //std::cout << "on_end_document()" << std::endl;
    xml_metachunk chunk;

    chunk.event = onDocEnd;

    chunks.push_back(chunk);
}

void artSaxParser::on_start_element(const Glib::ustring& name,
                                    const AttributeList& attributes)
{
    //std::cout << "node name=" << name << std::endl;
    xml_metachunk chunk;

    chunk.event = onElementStart;

    chunk.element.name = name;

    // Print attributes:
    for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
        //std::cout << "  Attribute " << iter->name << " = " << iter->value << std::endl;
        Tuple tuple;
        tuple.name = iter->name;
        tuple.value = iter->value;
        chunk.element.pairs.push_back(tuple);
    }

    chunks.push_back(chunk);
}

void artSaxParser::on_end_element(const Glib::ustring& name) {
    //std::cout << "on_end_element()" << std::endl;
    xml_metachunk chunk;

    (void)name; // тут не используется
    // предполагается что мы знаем кому принадлежит закрывающий элемент

    chunk.event = onElementEnd;

    chunks.push_back(chunk);
}

void artSaxParser::on_characters(const Glib::ustring& text) {
    //std::cout << "on_characters(): " << text << std::endl;
    
    size_t nulz = 0;
    for(size_t i = 0, l = text.length(); i < l; i++) {
        //printf("0x%02x\n", text.at(i));
        if(' ' == text.at(i) || '\n' == text.at(i)) nulz++;
    }
    if(nulz == text.length()) {
        // cout << "EMPTY CHARS" << endl;
        // просто игнорируем
    }
    else {
        // std::cout << "on_characters(): '" << text << "'" << std::endl;
        xml_metachunk chunk;

        chunk.event = onChars;
        chunk.str = text;

        chunks.push_back(chunk);
    }
}

void artSaxParser::on_comment(const Glib::ustring& text) {
    //std::cout << "on_comment(): " << text << std::endl;
    xml_metachunk chunk;

    chunk.event = onChars;
    chunk.str = text;

    chunks.push_back(chunk);
}

void artSaxParser::on_warning(const Glib::ustring& text)
{
    std::cout << "on_warning(): " << text << std::endl;
}

void artSaxParser::on_error(const Glib::ustring& text)
{
    std::cout << "on_error(): " << text << std::endl;
}

void artSaxParser::on_fatal_error(const Glib::ustring& text)
{
    std::cout << "on_fatal_error(): " << text << std::endl;
}

/////////////////////////////////////////////////////

string Element::fetchValue(string name) {
    // TODO: вообще-то тут надо заменить vector на map и не париться с перебором
    size_t i, l;
    for(i = 0, l = pairs.size(); i < l; i++)
        if(pairs[i].name == name) return pairs[i].value;

    return "<MISSING FIELD>";
}

/////////////////////////////////////////////////////

#include <assert.h>

#ifdef ART_DEBUG
void xml_metachunk::dump(void) {
    if (onDocStart == event) {
        cout << "onStartDoc" << endl;
    } else if (onDocEnd == event) {
        cout << "onEndDoc" << endl;
    } else if (onElementStart == event) {
        cout << "Element '" << element.name << "' in" << endl;
        size_t i, l;
        for (i = 0, l = element.pairs.size(); i < l; i++)
            cout << element.pairs[i].name << " = " << element.pairs[i].value
                 << endl;
    } else if (onElementEnd == event)
        cout << "Element out" << endl;
    else if (onChars == event) {
        cout << "CHARS '" << str << "'" << endl;
    } else if (onComment == event) {
        cout << "COMMENT '" << str << "'" << endl;
    } else
        assert(false);
}
#endif

/////////////////////////////////////////////////////

chunkGetter::chunkGetter() {}
chunkGetter::~chunkGetter() {}

void chunkGetter::copyChunks(vector <xml_metachunk> &chunks) {
    this->chunks = chunks;
    i = 0;
    l = this->chunks.size();
}

xml_metachunk chunkGetter::getChunk(void) {
    assert(i < l);

    xml_metachunk chunk = chunks[i];
    i++;
    return chunk;
}

xml_metachunk chunkGetter::getNonCharsChunk(void) {
    xml_metachunk chunk;

    do { chunk = getChunk(); } while (onChars == chunk.event);

    return chunk;
}

xml_metachunk chunkGetter::peekPrev(void) {
    assert(i > 0);

    return chunks[i - 1];
}

xml_metachunk chunkGetter::peekNext(void) {
    //	assert(i > 0 && ((i - 1) < l));
    //	return chunks[i + 1];
    // этот ^^^ код на самом деле смотрит на следующий за следующим

    return chunks[i];
}

void chunkGetter::putChunkBack(void) {
    assert(i > 0);

    i--;
}

void chunkGetter::seekUntilNonCharsChunk(void) {
    xml_metachunk chunk;

    do { chunk = getChunk(); } while (onChars == chunk.event);

    putChunkBack();
}

/////////////////////////////////////////////////////

#include "ccloader.h"

#include <stdlib.h> // exit()

int stringLoader(vector <string> &strings, chunkGetter &getter) {
    xml_metachunk chunk = getter.getChunk();

    assert(onElementStart == chunk.event);

    chunk = getter.getChunk();

    if(onChars == chunk.event) {
        // Тут onChars - как раз нужное содержимое элемента
        strings.push_back(chunk.str);

        chunk = getter.getChunk();

        assert(onElementEnd == chunk.event);

        return 0;
    }
    else if(onElementEnd) {
        // А тут нет значения, ничего не делаем вообще
        return 0;
    }
}

int exprLoader(CExpr &expr, chunkGetter &getter) {
    xml_metachunk chunk = getter.getChunk();

#ifdef ART_DEBUG
    if("expr" != chunk.element.name) chunk.dump();
#endif
    assert("expr" == chunk.element.name);

    local_record_t records[] = {
        {&expr.oper, "oper", CCSTR}
        ,{&expr.type, "type", CCSTR}
        ,{&expr.value, "value", CCSTR}
    };

    size_t nRecords = sizeof(records) / sizeof(local_record_t);
    ccload(chunk, records, nRecords);

    chunk = getter.getChunk();
    assert(onElementEnd == chunk.event);

    return 0;
}

int argsLoader(CArg &arg, chunkGetter &getter) {
    xml_metachunk chunk = getter.getChunk();

#ifdef ART_DEBUG
    if("args" != chunk.element.name) chunk.dump();
#endif
    assert("args" == chunk.element.name);

    local_record_t records[] = {
        {&arg.type, "type", CCSTR}
        ,{&arg.name, "name", CCSTR}
    };

    size_t nRecords = sizeof(records) / sizeof(local_record_t);
    ccload(chunk, records, nRecords);

    chunk = getter.getChunk();
    assert(onElementEnd == chunk.event);

    return 0;
}

int torLoader(CTor &tor, chunkGetter &getter) {
    xml_metachunk chunk = getter.getChunk();

#ifdef ART_DEBUG
    if("tor" != chunk.element.name) chunk.dump();
#endif
    assert("tor" == chunk.element.name);

    local_record_t records[] = {
        {&tor.name, "name", CCSTR}
        ,{&tor.float_arg_n1, "float_arg_n1", SIZET}
        ,{&tor.float_arg_n2, "float_arg_n2", SIZET}
        ,{&tor.is_handle_arg_out, "is_handle_arg_out", CCBOOL}
        ,{&tor.handle_arg_out, "handle_arg_out", SIZET}
        ,{&tor.cpp_alias, "cpp_alias", CCSTR}
    };

    size_t nRecords = sizeof(records) / sizeof(local_record_t);
    ccload(chunk, records, nRecords);

    bool inValidateDom_before = true;
    while(inValidateDom_before) {
        getter.seekUntilNonCharsChunk(); // игнорируем все комментарии
        stringLoader(tor.validateDom_before, getter);

        chunk = getter.getNonCharsChunk();
        if(chunk.event != onElementStart || chunk.element.name != "validateDom_before")
            inValidateDom_before = false;

        getter.putChunkBack();
    }

    // TODO: вставить дополнительную проверку если пропущен элемент validateDom_before

    bool inArgs = true;
    while(inArgs) {
        getter.seekUntilNonCharsChunk(); // игнорируем все комментарии

        CArg arg;

        argsLoader(arg, getter);

        tor.args.push_back(arg);

        //#ifdef ART_DEBUG
        //		cout << "TOR NAME: " << tor.name << endl;
        //		cout << "ADDED ARG:" << endl;
        //		arg.dump(4);
        //#endif

        chunk = getter.getNonCharsChunk();
        if(chunk.event != onElementStart || chunk.element.name != "args")
            inArgs = false;

        getter.putChunkBack();
    }

    CExpr expr;
    exprLoader(expr, getter);
    tor.badRetCode = expr;

    chunk = getter.getChunk();
    assert(onElementEnd == chunk.event); // конец tor'а

    return 0;
}

// 0 домен загружен
// 1 следующий блок не домен (блок возвращается в коробку)
int domainLoader(CDomain &dom, chunkGetter &getter) {
    xml_metachunk chunk;

    chunk = getter.getChunk();

#ifdef ART_DEBUG
    if("domain" != chunk.element.name) {
        cerr << "Expected element: domain, founded: '" << chunk.element.name
             << "'" << endl;
        chunk.dump();
        exit(-2);
    }
#endif
    assert("domain" == chunk.element.name);

    local_record_t records[] = {
        {&dom.name, "name", CCSTR}
        ,{&dom.float_handle, "float_handle", CCBOOL}
        ,{&dom.handle, "handle", CCSTR}
        ,{&dom.includesGlobal, "includesGlobal", CCBOOL}
        ,{&dom.ccLabel, "ccLabel", CCSTR}
    };


    size_t nRecords = sizeof(records) / sizeof(local_record_t);
    ccload(chunk, records, nRecords);

    if(dom.ccLabel == "<MISSING FIELD>")
        dom.ccLabel.clear();

    getter.seekUntilNonCharsChunk(); // игнорируем все комментарии

    CExpr expr;
    exprLoader(expr, getter);
    dom.bad_handle = expr;

    bool inIncludes = true;
    while(inIncludes) {
        getter.seekUntilNonCharsChunk(); // игнорируем все комментарии
        stringLoader(dom.includes, getter);

        chunk = getter.getNonCharsChunk();
        if(chunk.event != onElementStart || chunk.element.name != "includes")
            inIncludes = false;

        getter.putChunkBack();
    }

    // TODO: принадлежность CTor'а вынести из названия его переменной внутрь его
    // например, в перечисляемое поле torType

    bool inTors = true;
    while(inTors) {
        getter.seekUntilNonCharsChunk(); // игнорируем все комментарии

        chunk = getter.peekNext();
        assert(onElementStart == chunk.event);

        CTor tor;

        if("allocator" == chunk.element.fetchValue("__type__")) {
            torLoader(tor, getter);
            dom.allocators.push_back(tor);
        } else if("deallocator" == chunk.element.fetchValue("__type__")) {
            torLoader(tor, getter);
            dom.deallocators.push_back(tor);
        } else if("reallocator" == chunk.element.fetchValue("__type__")) {
            torLoader(tor, getter);
            dom.reallocators.push_back(tor);
        } else if("operator" == chunk.element.fetchValue("__type__")) {
            torLoader(tor, getter);
            dom.operators.push_back(tor);
        } else {
            cerr << "Unexpected element '" << chunk.element.fetchValue("__type__")
                 << "' !" << endl;
#ifdef ART_DEBUG
            chunk.dump();
#endif
            assert(false);
        }

        chunk = getter.getNonCharsChunk();

        if(chunk.event != onElementStart || chunk.element.name != "tor")
            inTors = false;

        getter.putChunkBack();
    }

    chunk = getter.getChunk();
    assert(onElementEnd == chunk.event); // конец domain'а

    return 0;
}

int saxparser_templateLoader(CTemplate &tpl, const char* filepath) {
    chunkGetter getter;

    try {
        artSaxParser parser;
        parser.set_substitute_entities(true); //
        parser.parse_file(filepath);

        getter.copyChunks(parser.chunks);
    } catch (const xmlpp::exception& ex) {
        std::cout << "libxml++ exception: " << ex.what() << std::endl;
        return -1;
    }

    xml_metachunk chunk;
    chunk = getter.getChunk();
    assert(onDocStart == chunk.event);

    chunk = getter.getChunk();
    //#ifdef ART_DEBUG
    //        chunk.dump();
    //#endif

    local_record_t records[] = {
        {&tpl.name, "name", CCSTR}
        ,{&tpl.prefix, "prefix", CCSTR}
        ,{&tpl.errlogmode, "errlogmode", CCSTR}
        ,{&tpl.trap_on_io_err, "trap_on_io_err", CCBOOL}
        ,{&tpl.remote_mode, "remote_mode", CCBOOL}
        ,{&tpl.force_flush, "force_flush", CCBOOL}
        ,{&tpl.fork_enabled, "fork_enabled", CCBOOL}
        ,{&tpl.multithreaded, "multithreaded", CCBOOL}
        ,{&tpl.art_start_selfinit, "art_start_selfinit", CCBOOL}
        ,{&tpl.threading, "threading", CCSTR}
        ,{&tpl.compiler_type, "compiler_type", CCSTR}
        ,{&tpl.trace_target, "trace_target", CCSTR}
    };

    size_t nRecords = sizeof(records) / sizeof(local_record_t);
    ccload(chunk, records, nRecords);

    bool inDomain = true;
    while(inDomain) {
        CDomain dom;

        getter.seekUntilNonCharsChunk(); // игнорируем все комментарии
        domainLoader(dom, getter);

        tpl.domains.push_back(dom);

        chunk = getter.getNonCharsChunk();
        if(chunk.event != onElementStart || chunk.element.name != "domain")
            inDomain = false;

        getter.putChunkBack();
    }

    chunk = getter.getChunk();

    assert(onElementEnd == chunk.event); // конец template'а

    // TODO: реализовать загрузку vector <CTor> specialTors;

#ifdef NOTHING
    // Demonstrate incremental parsing, sometimes useful for network connections:
    {
        //std::cout << "Incremental SAX Parser:" << std:endl;

        std::ifstream is(filepath.c_str());
        char buffer[64];

        artSaxParser parser;
        do {
            is.read(buffer, 63);
            Glib::ustring input(buffer, is.gcount());

            parser.parse_chunk(input);
        }
        while(is);

        parser.finish_chunk_parsing();
    }
#endif

    return 0;
}

#ifdef NOTHING_YET
#include "utils_linefetch.h"

int saxparser_templateLoader(CTemplate &tpl, CVLineFetcher *fetcher) {
    // TODO: NYI
    // В этот момент мне не тратить время на разбирание с последовательным
    // разбором (incremental parsing) XML, хотя это верный путь.
    // Обходной путь через подмену artSaxParser возможно, я реализую потом.
    // А сейчас я просто скину файл на диск и оттуда его разберу как файл :)
}
#endif
