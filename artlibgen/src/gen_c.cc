#include "gen_c.h"

#include <fstream>

std::string undef_str = "";

void	Generator_C_StubInfo(CTemplate &tpl, ofstream &file){
    file<<
           "/***************************************************************************"
        << endl;
    file<<"* Copyright (C) " << ART_VENDOR_DATE << " " << ART_VENDOR << endl;
    file<<"*" << endl;
    file<<"* THIS FILE GENERATED FROM TEMPLATE '" << tpl.name << "'" << endl;
    file<<"* BY ART LIBRARY GENERATOR VERSION " << ART_VERSION_BRANCH <<
          ART_VERSION_SUB << ART_VERSION_EXTRA << endl;
    file<<"*" << endl;
    file<<"* ART MARKED \"" << ART_DEVELNAME << "\"" << endl;
    file<<"*" << endl;
    file<<"* !!! DON\'T TOUCH THESE UNLESS YOU REALLY KNOW WHAT YOU'RE DOING !!!" << endl;
    file<<
           "****************************************************************************/"
        << endl << endl;
}

void	Generator_C_Header_Declaration(CTemplate &tpl, ofstream &head){
    CArg	tmpArg;
    CTor	tmpTor;
    CDomain tmpDom;
    size_t	dom_idx, tpl_domains_size,
            xtor_idx, tmpDom_allocators_size, tmpDom_deallocators_size,
            tmpDom_reallocators_size, tmpDom_operators_size,
            tmpTpl_specialTors_size,
            arg_idx, tmpTor_args_size;

    head << "/* foo decl BEGIN ***********************************/" << endl;
    tpl_domains_size = tpl.domains.size();
    for(dom_idx = 0; dom_idx < tpl_domains_size; dom_idx++){
        tmpDom = tpl.domains[dom_idx];

        // allocators // this code is original. other - clones.
        tmpDom_allocators_size = tmpDom.allocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_allocators_size; xtor_idx++){
            tmpTor = tmpDom.allocators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Скипать в С++-режиме
                break;
            }
            tmpArg = tmpTor.args[0];
            head << tmpArg.type << endl;
            head << tpl.prefix << tmpTor.name << "(";
            for(arg_idx = 1; arg_idx < tmpTor.args.size(); arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.type << " " << tmpArg.name << ", ";
            }
            head << "const char *file, size_t line);" << endl << endl;
        }

        // deallocators
        tmpDom_deallocators_size = tmpDom.deallocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_deallocators_size; xtor_idx++){
            tmpTor = tmpDom.deallocators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }
            tmpArg = tmpTor.args[0];
            head << tmpArg.type << endl;
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.type << " " << tmpArg.name << ", ";
            }
            head << "const char *file, size_t line);" << endl << endl;
        }

        // reallocators
        tmpDom_reallocators_size = tmpDom.reallocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_reallocators_size; xtor_idx++){
            tmpTor = tmpDom.reallocators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }
            tmpArg = tmpTor.args[0];
            head << tmpArg.type << endl;
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.type << " " << tmpArg.name << ", ";
            }
            head << "const char *file, size_t line);" << endl << endl;
        }

        // operators
        tmpDom_operators_size = tmpDom.operators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_operators_size; xtor_idx++){
            tmpTor = tmpDom.operators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }
            tmpArg = tmpTor.args[0];
            head << tmpArg.type << endl;
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.type << " " << tmpArg.name << ", ";
            }
            head << "const char *file, size_t line);" << endl << endl;
        }
    }

    // specialTors
    tmpTpl_specialTors_size = tpl.specialTors.size();
    for(xtor_idx = 0; xtor_idx < tmpTpl_specialTors_size; xtor_idx++){
        tmpTor = tpl.specialTors[xtor_idx];
        if(tmpTor.cpp_alias != ""){
            // Пропускать в С++-режиме
            break;
        }
        tmpArg = tmpTor.args[0];
        head << tmpArg.type << endl;
        head << tpl.prefix << tmpTor.name << "(";
        tmpTor_args_size = tmpTor.args.size();
        for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
            tmpArg = tmpTor.args[arg_idx];
            head << tmpArg.type << " " << tmpArg.name << ", ";
        }
        head << "const char *file, size_t line);" << endl << endl;
    }

    head << "/* foo decl END *************************************/" << endl << endl;
}

void	Generator_C_Header_Redefenition(CTemplate &tpl, ofstream &head){
    CArg	tmpArg;
    CTor	tmpTor;
    CDomain tmpDom;
    size_t	dom_idx, tpl_domains_size,
            xtor_idx, tmpDom_allocators_size, tmpDom_deallocators_size,
            tmpDom_reallocators_size, tmpDom_operators_size,
            tmpTpl_specialTors_size,
            arg_idx, tmpTor_args_size;

    head << "/* foo redef BEGIN **************************************/" << endl;
    head << "#ifndef ART_NO_REDEF" << endl;
    head << "#if __INCLUDE_LEVEL__ == 0" << endl;


    if(!tpl.fork_enabled) {
        undef_str += "#undef fork\n"; 
        head << "#define fork()\t" << tpl.prefix <<
                "fork_forbidden(__FILE__, __LINE__);" << endl;
    }


    tpl_domains_size = tpl.domains.size();
    for(dom_idx = 0; dom_idx < tpl_domains_size; dom_idx++){
        tmpDom = tpl.domains[dom_idx];

        if(tmpDom.ccLabel.size())
            head << "#ifdef " << tmpDom.ccLabel << endl;

        // allocators // this code is original. other - clones.
        tmpDom_allocators_size = tmpDom.allocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_allocators_size; xtor_idx++){
            tmpTor = tmpDom.allocators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }

            // left part
            undef_str += "#undef " + tmpTor.name + "\n";
            head << "#define " << tmpTor.name << "(";


            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size - 1; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            tmpArg = tmpTor.args[arg_idx];
            head << tmpArg.name << ")\t";

            // right part
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            head << "__FILE__, __LINE__)" << endl;
        
        }

        // deallocators
        tmpDom_deallocators_size = tmpDom.deallocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_deallocators_size; xtor_idx++){
            tmpTor = tmpDom.deallocators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }

            // left part
            undef_str += "#undef " + tmpTor.name + "\n";
            head << "#define " << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size - 1; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            tmpArg = tmpTor.args[arg_idx];
            head << tmpArg.name << ")\t";

            // right part
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            head << "__FILE__, __LINE__)" << endl;
        }

        // reallocators
        tmpDom_reallocators_size = tmpDom.reallocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_reallocators_size; xtor_idx++){
            tmpTor = tmpDom.reallocators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }

            // left part
            undef_str += "#undef " + tmpTor.name + "\n";
            head << "#define " << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size - 1; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            tmpArg = tmpTor.args[arg_idx];
            head << tmpArg.name << ")\t";

            // right part
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            head << "__FILE__, __LINE__)" << endl;
        }

        // operators
        tmpDom_operators_size = tmpDom.operators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_operators_size; xtor_idx++){
            tmpTor = tmpDom.operators[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }

            // left part
            undef_str += "#undef " + tmpTor.name + "\n";
            head << "#define " << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size - 1; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            tmpArg = tmpTor.args[arg_idx];
            head << tmpArg.name << ")\t";

            // right part
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            head << "__FILE__, __LINE__)" << endl;
        }

        // specialTors
        tmpTpl_specialTors_size = tpl.specialTors.size();
        for(xtor_idx = 0; xtor_idx < tmpTpl_specialTors_size; xtor_idx++){
            tmpTor = tpl.specialTors[xtor_idx];
            if(tmpTor.cpp_alias != ""){
                // Пропускать в С++-режиме
                break;
            }

            // left part
            undef_str += "#undef " + tmpTor.name + "\n";
            head << "#define " << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size - 1; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            tmpArg = tmpTor.args[arg_idx];
            head << tmpArg.name << ")\t";

            // right part
            head << tpl.prefix << tmpTor.name << "(";
            tmpTor_args_size = tmpTor.args.size();
            for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
                tmpArg = tmpTor.args[arg_idx];
                head << tmpArg.name << ", ";
            }
            head << "__FILE__, __LINE__)" << endl;
        }
        
        if(tmpDom.ccLabel.size())
            head << "#endif" << endl << endl;
    }

    head << "#endif" << endl;
    head << "/* foo redef END ****************************************/" << endl;
    head << "#endif" << endl;
}

void	Generator_C_Header(CTemplate &tpl, ofstream &head){
    Generator_C_StubInfo(tpl, head);
    size_t	t, /*i,*/ tpl_domains_size/*, tmpDom_includes_size*/;

    head << "#ifndef __pd_" << tpl.prefix << "h__" << endl;
    head << "#define __pd_" << tpl.prefix << "h__" << endl << endl;


    head << "#ifndef __cplusplus" << endl;


    if(tpl.multithreaded){
        if(tpl.threading == "posix"){
            head << "#include <pthread.h>" << endl;
        }
        else if(tpl.threading == "win32"){
            head << "#include <Windows.h>" << endl;
            head << "#include <Winbase.h>" << endl;
        }
    }
    head << endl;

    tpl_domains_size = tpl.domains.size();
/*    for(i = 0; i < tpl_domains_size; i++){
        CDomain tmpDom;
        tmpDom = tpl.domains[i];
        tmpDom_includes_size = tmpDom.includes.size();
        for(t = 0; t < tmpDom_includes_size; t++){
            head << "#include ";

            if(tmpDom.includesGlobal) head << "<";
            else head << "\"";

            head << tmpDom.includes[t];

            if(tmpDom.includesGlobal) head << ">";
            else head << "\"";

            head << endl;
        }
    }*/
    head << endl;

    head << "#include <stdio.h>" << endl;   // art uses it
    head << "#include <stdarg.h>" << endl;  // art uses it


#if 0
    if(tpl.compiler_type == "win32"){
        if(tpl.remote_mode){
            head << "#include <winsock2.h>" << endl;
        } else {
            // ???
        }
    } else if(tpl.compiler_type == "gcc") {
        if(tpl.remote_mode){
            head << "#include <sys/types.h>" << endl;
            head << "#include <sys/socket.h>" << endl;
            head << "#include <sys/errno.h> /* ECONNREFUSED */" << endl;
            head << "#include <netdb.h>" << endl;
            head << "#include <netinet/in.h>" << endl;
        } else {
            // ???
        }
        head << "#include <stdarg.h>" << endl;	// art uses it
        head << "#include <stdio.h>" << endl;	// art uses it
        head << "#include <stdlib.h> /* exit() */" << endl;	// art uses it
        head << "#include <string.h> /* strlen() */" << endl;// art uses it
        head << "#include <unistd.h> /* unlink() */" << endl;// art uses it
    } else {
        cerr << "NYI at " << __FILE__ << ":" << __LINE__ << endl;
    }
#endif



    if(tpl.compiler_type == "gcc"){
        head << "typedef unsigned long long artuint64;" << endl;
    }
    else if(tpl.compiler_type == "win32"){
        head << "typedef unsigned __int64 artuint64;" << endl;
    }

    if(tpl.multithreaded){
        if(tpl.threading == "posix"){
            head << "extern pthread_mutex_t art_mutex;" << endl;
        }
        else if(tpl.threading == "win32"){
            head << "HANDLE art_mutex;" << endl;
        }
    }

    if(tpl.remote_mode){
        head << "extern int\tart_socket;" << endl;
    }
    else	{
        head << "extern FILE*\tart_tracefile;" << endl;
    }
    head << "extern int\tart_stop_called;" << endl;
    head << "extern int\tart_retval;" << endl;
    if(tpl.art_start_selfinit) {
        head << "extern char\tart_start_initialized;" << endl;
    }
    head << endl;

    head << "char*\tartsize_t2str(size_t size);" << endl;
    for(t = 1; t <= ART_NUMBER_ARTUINT64TOSTR; t++){
        head << "char*\tartuint64tostr" << t << " (artuint64 value);" << endl;
    }
    head << "void\tarterrlog(char *msg, const char *file, size_t line);" << endl;
    head << "void\tart_trace(int n, ...);" << endl;
    head << "void\tart_start(const char *appname);" << endl;
    head << "void\tart_stop(void);" << endl << endl;

    Generator_C_Header_Declaration(tpl, head);
    Generator_C_Header_Redefenition(tpl, head);
    head << endl;

    head << "#endif" << endl;
    head << "#endif" << endl;
}

#include <string.h> // stncpy() в Generator_C_Source_ART_Part()

int	Generator_C_Source_ART_Part(CTemplate &tpl, char *tplfilename,
                                ofstream &src){
    Generator_C_StubInfo(tpl, src);
    size_t	t;
    CTor    tmpTor;

    src << "#define ART_NO_REDEF" << endl;
    src << "#include \"art.h\"" << endl << endl;



    src << undef_str << endl << endl;


    if(tpl.compiler_type == "win32"){
        if(tpl.remote_mode){
            src << "#include <winsock2.h>" << endl;
        } else {
            // ???
        }
    } else if(tpl.compiler_type == "gcc") {
        if(tpl.remote_mode){
            src << "#include <sys/types.h>" << endl;
            src << "#include <sys/socket.h>" << endl;
            src << "#include <sys/errno.h> /* ECONNREFUSED */" << endl;
            src << "#include <netdb.h>" << endl;
            src << "#include <netinet/in.h>" << endl;
        } else {
            // ???
        }
        src << "#include <stdlib.h> /* exit() */" << endl; // art uses it
        src << "#include <string.h> /* strlen() */" << endl;// art uses it
        src << "#include <unistd.h> /* unlink() */" << endl;// art uses it
    } else {
        cerr << "NYI at " << __FILE__ << ":" << __LINE__ << endl;
    }


    if(tpl.multithreaded) {
        if("posix" == tpl.threading) {
            src << "pthread_mutex_t art_mutex = PTHREAD_MUTEX_INITIALIZER;"
                << endl;
        }
    }

    if(tpl.remote_mode){
        src << "int\tart_socket;" << endl;
    }
    else	{
        src << "FILE*\tart_tracefile;" << endl;
    }
    src << "int\tart_stop_called;" << endl;
    src << "int\tart_retval;" << endl;

    if(tpl.art_start_selfinit) {
        src << "char art_start_initialized = 0;" << endl;
    }

    src << endl;

    src << "char*\tartsize_t2str(size_t size){" << endl;
    src << "static char buff[32];" << endl;
    src << "sprintf(buff, \"%u\", size);" << endl;
    src << "return(buff);" << endl;
    src << "}" << endl << endl;

    for(t = 1; t <= ART_NUMBER_ARTUINT64TOSTR; t++){
        src << "char*\tartuint64tostr" << t << "(artuint64 value){" << endl;
        src << "static char\tbuff[32];" << endl;
        src << "artuint64\ta, b;" << endl;
        src << "size_t\ti;" << endl;
        src << "a = value;" << endl;
        src << "for(i = 0; i < 16; i++){" << endl;
        src << "\tb = a % 16;" << endl;
        src << "\ta /= 16;" << endl;
        src << "\tif(b < 10) {" << endl;
        src << "\t\tbuff[15 - i] = '0' + b;" << endl;
        src << "\t\t}" << endl;
        src << "\telse	{" << endl;
        src << "\t\tb -= 10;" << endl;
        src << "\t\tbuff[15 - i] = 'A' + b;" << endl;
        src << "\t\t}" << endl;
        src << "\t}" << endl;
        src << "buff[16] = 0;" << endl;
        src << "return(buff);" << endl;
        src << "}" << endl << endl;
    }

    if(tpl.remote_mode){
        src << "int\tLTNet_ConnectClient(char *host, unsigned short port){" << endl;
        src << "struct addrinfo hints;" << endl;
        src << "struct addrinfo *res;" << endl;
        src << "char port_str[6];" << endl;
        src << "int resoult;" << endl;
        src << "int sd;" << endl;
        src << "int was_refused;" << endl;
        src << "int econn_refused_state;" << endl << endl;

        src << "memset(&hints, 0, sizeof(hints));" << endl;
        src << "hints.ai_family = AF_UNSPEC;" << endl;
        src << "hints.ai_protocol = IPPROTO_TCP;" << endl;
        src << "hints.ai_socktype = SOCK_STREAM;" << endl << endl;

        src << "snprintf(port_str, sizeof(port_str), \"%u\", port);" << endl;
        src << "resoult = getaddrinfo(host, port_str, &hints, &res);" << endl;
        src << "if(0 != resoult) return -1;" << endl;
        src << "else\t{" << endl;
        src << "\twhile(res){" << endl;
        src << "\t\tsd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);" << endl;
        //src << "\t\tsd = socket(res.ai_family, res.ai_socktype, res.ai_protocol);" << endl;
        src << "\t\tif(sd < 0){freeaddrinfo(res); return -1;}" << endl;
        src << "\t\tresoult = connect(sd, res->ai_addr, res->ai_addrlen);" << endl;
        src << "\t\tif(0 == resoult){was_refused = 0; break;}" << endl;
        src << "\t\tif(resoult < 0){" << endl;
        src << "\t\t\tswitch(errno){" << endl;
        src << "\t\t\t\tcase ECONNREFUSED:" << endl;
        src << "\t\t\t\tswitch (econn_refused_state){" << endl;
        src << "\t\t\t\t\t\tcase 0:" << endl;
        src << "\t\t\t\t\t\tcase 1:" << endl;
        src << "\t\t\t\t\t\twas_refused = 1;" << endl;
        src << "\t\t\t\t\t\t}" << endl;
        src << "\t\t\t\t\tbreak;" << endl;
        src << "\t\t\t\t\t}" << endl;
        src << "\t\t\t\t}" << endl;
        src << "\t\t\tclose(sd);" << endl;
        src << "\t\t\tres = res->ai_next;" << endl;
        src << "\t\t\t}" << endl;
        src << "\t\tfreeaddrinfo(res);" << endl;
        src << "\t\t}" << endl;
        src << "\tif(0 == resoult) return sd;" << endl;
        src << "\telse if(was_refused) return -1;" << endl;
        src << "\telse return -1;" << endl;
        src << "}" << endl << endl;
    }

    // NOTE: char *file, size_t line used only in console-mode
    src << "void\tarterrlog(char *msg, const char *file, size_t line){" << endl;
    if(tpl.errlogmode == ""){
        src << "}" << endl << endl;
    }
    else if(tpl.errlogmode == "console"){
        src << "perror(msg); fprintf(stderr, \"%s:%u\\n\", file, line);" << endl
            << "}" << endl << endl;
    }
    else	{
        src << "FILE *fl;" << endl;
        src << "fl = fopen(\"" << tpl.errlogmode << "\", \"a\");" << endl;
        src << "fwrite(msg, strlen(msg), 1, fl);" << endl;
        src << "fwrite(\" \", 1, 1, fl);" << endl;
        src << "fwrite(file, strlen(file), 1, fl);" << endl;
        src << "fwrite(\":\", 1, 1, fl);" << endl;
        src << "fwrite(artsize_t2str(line), strlen(artsize_t2str(line)), 1, fl);" << endl;
        // TODO: использование функции artsize_t2str() возможно небезопасно, т.к. надо быть
        // уверенным что эту функцию до нас никто не вызывал и что не будут уничтожены данные
        src << "fwrite(\"\\n\", 1, 1, fl);" << endl;
        src << "fflush(fl);" << endl;
        src << "fclose(fl);" << endl;
        src << "}" << endl << endl;
    }

    src << "void\tart_trace(int n, ...){" << endl;
    if(tpl.remote_mode){
        if(tpl.trap_on_io_err){
            src << "int art_retval;" << endl;
        }
        src << "int count; char *tmpstr; va_list ap; va_start(ap, n);" << endl;
        src << "for(count = 1; count < n; count++){" << endl;
        src << "\ttmpstr = va_arg(ap, char *);" << endl;
        if(tpl.trap_on_io_err) src << "\tart_retval = "; else src<<"\t";
        src << "send(art_socket, tmpstr, strlen(tmpstr), 0);" << endl;
        if(tpl.trap_on_io_err){
            src << "\tif(art_retval < 0){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
        if(tpl.trap_on_io_err) src << "\tart_retval = "; else src<<"\t";
        src << "send(art_socket, \" \", 1, 0);" << endl;
        if(tpl.trap_on_io_err){
            src << "\tif(art_retval < 0){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
        src << "\t}" << endl;
        src << "tmpstr = va_arg(ap, char *);" << endl;
        if(tpl.trap_on_io_err) src << "art_retval = "; else src<<"\t";
        src << "send(art_socket, tmpstr, strlen(tmpstr), 0);" << endl;
        if(tpl.trap_on_io_err){
            src << "if(art_retval < 0){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
        src << "va_end(ap);" << endl;
        if(tpl.trap_on_io_err) src << "art_retval = ";
        src << "send(art_socket, \"\\n\", 1, 0);" << endl;
        if(tpl.trap_on_io_err){
            src << "if(art_retval < 0){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
    }
    else	{
        if(tpl.trap_on_io_err){
            src << "int art_retval;" << endl;
        }
        src << "int count; char *tmpstr; va_list ap; va_start(ap, n);" << endl;
        src << "for(count = 1; count < n; count++){" << endl;
        src << "\ttmpstr = va_arg(ap, char *);" << endl;
        if(tpl.trap_on_io_err) src << "\tart_retval = "; else src<<"\t";
        src << "fwrite(tmpstr, strlen(tmpstr), 1, art_tracefile);" << endl;
        if(tpl.trap_on_io_err){
            src << "\tif(art_retval != 1){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
        if(tpl.trap_on_io_err) src << "\tart_retval = "; else src<<"\t";
        src << "fwrite(\" \", 1, 1, art_tracefile);" << endl;
        if(tpl.trap_on_io_err){
            src << "\tif(art_retval != 1){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
        src << "\t}" << endl;
        src << "tmpstr = va_arg(ap, char *);" << endl;
        if(tpl.trap_on_io_err) src << "art_retval = "; else src<<"\t";
        src << "fwrite(tmpstr, strlen(tmpstr), 1, art_tracefile);" << endl;
        if(tpl.trap_on_io_err){
            src << "if(art_retval != 1){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
        src << "va_end(ap);" << endl;
        if(tpl.trap_on_io_err) src << "art_retval = ";
        src << "fwrite(\"\\n\", 1, 1, art_tracefile);" << endl;
        if(tpl.trap_on_io_err){
            src << "if(art_retval != 1){arterrlog(\"fwrite()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
        }
        if(tpl.force_flush){
            if(tpl.trap_on_io_err) src << "art_retval = ";
            src << "fflush(art_tracefile);" << endl;
            if(tpl.trap_on_io_err){
                src << "if(art_retval != 0){arterrlog(\"fflush()\", __FILE__, __LINE__); exit(art_retval);}" << endl;
            }
        }
    }
    src << "}" << endl << endl;

    src << "void\tart_start(const char *appname){" << endl;
    if(tpl.multithreaded){
        if(tpl.threading == "posix"){
            src << "int retval;" << endl;
        }
        else if(tpl.threading == "win32"){/* nothing */}
    }
    if(tpl.errlogmode != "" && tpl.errlogmode != "console")
        src << "unlink(\"" << tpl.errlogmode << "\");" << endl;
    src << "art_stop_called = 0;" << endl;
    if(tpl.multithreaded){
        /* if(tpl.threading == "posix"){
            // Не инициализируем объект синхронизации динамически, т.к. он
            // инициализирован статически
            src << "rez = pthread_mutex_init(&art_mutex, NULL);" << endl;
            src << "if(rez){arterrlog(\"pthread_mutex_init()\", __FILE__, __LINE__); exit(-2);}"<<endl;
        }
        else */
        if(tpl.threading == "win32"){
            src << "art_mutex = CreateMutex(NULL, FALSE, NULL);" << endl;
            src << "if(!art_mutex){arterrlog(\"CreateMutex()\", __FILE__, __LINE__); exit(-2);}" << endl;
        }
    }
    src << endl;

    //TODO: тут нужно блокировать art_mutex сразу и делать свои дела

    if(tpl.art_start_selfinit) {
        if(tpl.multithreaded) {
            if("posix" == tpl.threading) {
                // art_mutex уже инициализирован
                src << "retval = pthread_mutex_lock(&art_mutex);" << endl;
                src << "if(retval){arterrlog(\"pthread_mutex_lock()\", __FILE__, __LINE__); exit(-2);}" << endl;
                src << "if(art_start_initialized) {" << endl;
                src << "\tretval = pthread_mutex_unlock(&art_mutex);" << endl;
                src << "\tif(retval){arterrlog(\"pthread_mutex_lock()\", __FILE__, __LINE__); exit(-2);}" << endl;
                src << "\treturn;" << endl;
                src << "}" << endl;
                src << "art_start_initialized = 1;" << endl;
//                src << "retval = pthread_mutex_unlock(&art_mutex);" << endl;
//                src << "if(retval){arterrlog(\"pthread_mutex_lock()\", __FILE__, __LINE__); exit(-2);}" << endl;
            } else if("win32" == tpl.threading) {
                src << "#error NYI! art_mutex not initialized yet" << endl;
            }
        } else {
            src << "if(art_start_initialized) return;" << endl;
            src << "art_start_initialized = 1;" << endl << endl;
        }
    }

    if(tpl.remote_mode){
        // connect() to server

        // опасный код, тут проверок почти нет
        char host[100];
        char port[8];
        char *tmp;

        strncpy(host, tpl.trace_target.c_str(), 99);
        host[99] = 0;
        strncpy(port, strchr(host, ':') + 1, 7);
        port[7] = 0;
        tmp = strchr(host, ':');
        *tmp = 0;
        // опасный код, тут проверок почти нет

        src << "art_socket = LTNet_ConnectClient(\"" << host << "\", " << port << ");" << endl;
        //src << "art_socket = LTNet_ConnectClient(" << tpl.trace_target << ");" << endl;
        src << "if(art_socket < 0){arterrlog(\"LTNet_ConnectClient()\", __FILE__, __LINE__);" << endl;
        src << "\texit(art_socket);}" << endl;
    }
    else	{
        // fopen() tracefile
        src << "art_tracefile = fopen(\"" << tpl.trace_target << "\", \"w\");" << endl;
        src << "if(art_tracefile == NULL){arterrlog(\"fopen()\", __FILE__, __LINE__);exit(-1);}" << endl;
    }

    src << "art_trace(1, \"ART TRACING BEGIN\");" << endl;
    src << "art_trace(1, \"ART PROTOCOL 01\");" << endl;

    //////////////////////////////////////////////////////////////////////
    // Now we put template to source
    // Later it can be readed
    Generator_C_Source_ART_Template_Writer(tplfilename, src);
    //////////////////////////////////////////////////////////////////////

    src << "art_retval = atexit((void *)art_stop);" << endl;
    src << "if(art_retval == -1){arterrlog(\"atexit()\", __FILE__, __LINE__);exit(-1);}" << endl;
    
    src << "retval = pthread_mutex_unlock(&art_mutex);" << endl;
    src << "if(retval){arterrlog(\"pthread_mutex_lock()\", __FILE__, __LINE__); exit(-2);}" << endl;

    src << "}" << endl << endl;

    src << "void\tart_stop(void){" << endl;
    if(tpl.multithreaded){
        if(tpl.threading == "posix"){
            src << "int rez;" << endl;
        }
        else if(tpl.threading == "win32"){
            src << "BOOL rez;" << endl;
        }
    }
    src << "if(art_stop_called){arterrlog(\"ART: loop detected! abort()!!!\", __FILE__, __LINE__);abort();}" << endl;
    src << "else{art_stop_called = 1;}" << endl;
    if(tpl.multithreaded){
        /* if(tpl.threading == "posix"){
            // Не деинициализируем объект синхронизации динамически, т.к. он
            // инициализирован статически
            // TODO: и не требует деинцилизации в этом случае?
            src << "rez = pthread_mutex_destroy(&art_mutex);" << endl;
            src << "if(rez != 0){arterrlog(\"pthread_mutex_destroy()\", __FILE__, __LINE__);exit(-1);}" << endl;
        }
        else */
        if(tpl.threading == "win32"){
            src << "rez = CloseHandle(art_mutex);" << endl;
            src << "if(!rez){arterrlog(\"CloseHandle()\", __FILE__, __LINE__);exit(-1);}" << endl;
        }
    }
    src << "art_trace(1, \"ART TRACING DONE\");" << endl;
    if(tpl.remote_mode){
        src << "shutdown(art_socket, SHUT_RDWR);" << endl;
        src << "close(art_socket);" << endl;
    }
    else	{
        src << "fflush(art_tracefile);" << endl;
        src << "fclose(art_tracefile);" << endl;
    }
    src << "}" << endl << endl;
    // NOTE: we shouldn't close errlogfile coz it's closed every time after
    // fopen(), fwrite()

    if(!tpl.fork_enabled) {
        src << "pid_t " << tpl.prefix << "fork_forbidden(const char *" <<
            tpl.prefix << "file, size_t " << tpl.prefix << "line) {" << endl;
        src << "art_trace(1, \"ART_TRAP\");" << endl;
        src << "art_trace(1, \"fork()\");" << endl;
        src << "art_trace(1, " << "art_file);" << endl;
        src << "art_trace(1, artsize_t2str(art_line));" << endl;
        src << "abort();" << endl;
        src << "}" << endl << endl;
    }

    return 0;
}

void escape_line(string &line) {
    string newLine;
    char theChar;
    
    for(size_t i = 0, l = line.length(); i < l; i++) {
        theChar = line[i];
        if('"' == theChar) { newLine += "\\\""; }
        else newLine += theChar;
    }
    
    line = newLine;
}

#include "utils_linefetch.h"

int     Generator_C_Source_ART_Template_Writer(char *tplfilename,
                                               ofstream &src) {
    string line;
    CVLineFetcher *fetcher;

    fetcher = new CFileLineFetcher;
    fetcher->attach(tplfilename);
    while(true) {
        try {
            fetcher->fetchLine(line);
        }
        catch(...) {return 0;}

        if(line.length()) {
            escape_line(line);
            src << "art_trace(1, \"" << line << "\");" << endl;
        }
        // в противном случае ничего не пишем, ибо писатель умирает
        // на пустых строках
    }

    return 0;
}

int	Generator_C_Source_USER_Body_Part(CTemplate &tpl, CDomain &tmpDom,
                                      CTor &tmpTor, ofstream &src){
    int	retval;
    retval = Generator_C_USER_Body_Prolog(tpl, tmpDom, tmpTor, src);
    if(retval) return(retval);
    retval = Generator_C_USER_Body_Validation(tpl, tmpDom, tmpTor, src);
    if(retval) return(retval);
    retval = Generator_C_USER_Body_Action(tpl, tmpDom, tmpTor, src);
    if(retval) return(retval);
    retval = Generator_C_USER_Body_Epilog(tpl, tmpDom, tmpTor, src);
    if(retval) return(retval);
    return(0);
}

int	Generator_C_Source_USER_Part(CTemplate &tpl, ofstream &src){
    CArg	tmpArg;
    CTor	tmpTor;
    CDomain tmpDom;
    size_t	dom_idx, tpl_domains_size,
            xtor_idx, tmpDom_allocators_size, tmpDom_deallocators_size,
            tmpDom_reallocators_size, tmpDom_operators_size,
            tmpTpl_specialTors_size;

    src << "/* *** BEGIN **************************************/" << endl;
    tpl_domains_size = tpl.domains.size();
    for(dom_idx = 0; dom_idx < tpl_domains_size; dom_idx++){
        tmpDom = tpl.domains[dom_idx];
        tmpDom_allocators_size = tmpDom.allocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_allocators_size; xtor_idx++){
            tmpTor = tmpDom.allocators[xtor_idx];
            Generator_C_Source_USER_Body_Part(tpl, tmpDom, tmpTor, src);
        }

        tmpDom_reallocators_size = tmpDom.reallocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_reallocators_size; xtor_idx++){
            tmpTor = tmpDom.reallocators[xtor_idx];
            Generator_C_Source_USER_Body_Part(tpl, tmpDom, tmpTor, src);
        }

        tmpDom_operators_size = tmpDom.operators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_operators_size; xtor_idx++){
            tmpTor = tmpDom.operators[xtor_idx];
            Generator_C_Source_USER_Body_Part(tpl, tmpDom, tmpTor, src);
        }

        tmpDom_deallocators_size = tmpDom.deallocators.size();
        for(xtor_idx = 0; xtor_idx < tmpDom_deallocators_size; xtor_idx++){
            tmpTor = tmpDom.deallocators[xtor_idx];
            Generator_C_Source_USER_Body_Part(tpl, tmpDom, tmpTor, src);
        }
    }

    tmpTpl_specialTors_size = tpl.specialTors.size();
    for(xtor_idx = 0; xtor_idx < tmpTpl_specialTors_size; xtor_idx++){
        tmpTor = tpl.specialTors[xtor_idx];
        Generator_C_Source_USER_Body_Part(tpl, tmpDom, tmpTor, src);
    }

    src << "/* *** END ****************************************/" << endl;
    return (0);
}

int	Generator_C_Source(CTemplate &tpl, char *tplfilename, ofstream &src){
    int retval;
    retval = Generator_C_Source_ART_Part(tpl, tplfilename, src);
    if(retval) return (retval);
    retval = Generator_C_Source_USER_Part(tpl, src);
    return (retval);
}

int	Generator_C(CTemplate &tpl, char *tplfilename,
                const char *header_file, const char *source_file){
    int retval;
    ofstream head(header_file), source(source_file);
    Generator_C_Header(tpl, head);
    retval = Generator_C_Source(tpl, tplfilename, source);
    return (retval);
}
