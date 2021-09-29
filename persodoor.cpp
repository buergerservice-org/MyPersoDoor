// workflowClient.cpp : Test of workflowLibrary

// Copyright (C) 2021 buergerservice.org e.V. <KeePerso@buergerservice.org>

// Distributed under the Boost Software License, Version 1.0.

// (See accompanying file LICENSE_1_0.txt or copy at

// https://www.boost.org/LICENSE_1_0.txt)





#include <iostream>

#include <cstdio>

#include <memory>

#include <stdexcept>

#include <string>

#include <array>

#include <chrono>

#include <thread>

#include <iostream>

#include <time.h>

#include <algorithm>

//for workflowLibraryLinux.a

#include "workflowLibrary.h"









std::wstring strtowstr(const std::string& s)

{

    std::wstring wsTmp(s.begin(), s.end());

    //ws = wsTmp;

    return wsTmp;

}





std::string commandexec(const char* cmd) {

    std::array<char, 128> buffer;

    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) 
    {

        throw std::runtime_error("popen() failed!");

    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
    {

        result += buffer.data();

    }

    return result;

}



void writelog(FILE *l, std::string s){

    time_t rawtime;

    struct tm * timeinfo;

    std::string c;

    char buffer[80];

    time(&rawtime);

    timeinfo=localtime(&rawtime);

    //printf("local time and date: %s", ctime(&rawtime));

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);

    std::string str(buffer);

    c=str +": " + s;  

    fprintf(l, c.c_str());

}


struct isSpace
{
    bool operator()(unsigned c)
    {
        return (c == ' ' || c == '\n' || c == '\r' ||
                c == '\t' || c == '\v' || c == '\f');
    }
};



int main(int argc, char** argv)

{
    std::string PINstring = "";
    std::strin
    std::string workflowoutputstring = "";
    //std::basic_string<TCHAR> textstr;

    //std::locale::global(std::locale("German_germany.UTF-8"));

    FILE *log;
    FILE *hashkey;
    FILE *fp;
    FILE *PINfile;
    char line[1035];
    char mystring[100];
    char pinstr[100];


    time_t rawtime;
    struct tm * timeinfo;


    log = fopen("persodoor.log", "a+");

    hashkey = fopen("hashkey.txt", "r");
    PINfile=fopen("PIN.txt", "r");

    writelog(log, "persodoor started.\n"); 

    

    //instantiate a new workflowclass

    workflowLibrary::workflow wf;
    printf("local time and date: %s", ctime(&rawtime));


    std::cout << "\nbuergerservice.org e.V. persodoor.\n\n" << std::endl;

    // Check command line arguments.
/*
    if (argc != 2)

    {

        //outputstring = commandexec("ls -l");

        std::cout << outputstring << std::endl;

        std::cerr <<

            "Usage: ./persodoor <PIN>\n" <<

            "Example:\n" <<

            "./persodoor 123456\n";

        return EXIT_FAILURE;

    }

    PINstring = argv[1];
*/

    if(PINfile==NULL) perror ("Error opening file PIN.txt");
    else
    {

       if(fgets(pinstr, 100, PINfile)!=NULL)
       {
          //puts(mystring);
          std::cout << "PIN.txt read" << std::endl;

       }

       else
       {
          std::cout << "cant read PIN.txt" << std::endl;
       }
    }

    std::cout << "PIN from file : " << std::string(pinstr) << "Ende" << std::endl;


    PINstring=std::string(pinstr);
    PINstring.erase(std::remove_if(PINstring.begin(), PINstring.end(), isSpace()), PINstring.end());
    //std::cout << "PIN= " << PINstring << std::endl;


    while(true)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "starting startworkflow." << std::endl;
        writelog(log, "startworkflow.\n");

        //start the workflow of the new workflowclass and show output

        ////workflowoutputstring = wf.startworkflow(PINstring);

        //workflowoutputstring="123456";

        //fprintf(producedhashkey, workflowoutputstring.c_str());

        //std::cout << "starting startworkflow." << std::endl;

        //start the workflow of the new workflowclass and show output

        workflowoutputstring = wf.startworkflow(PINstring);


        outputstring = workflowoutputstring;
        if (outputstring == "e1")
        {
            std::cout << "ERROR - please check AusweisApp2, cardreader and Personalausweis!Exiting Plugin." << std::endl;
            return EXIT_FAILURE;
        }

        else if (outputstring == "e2")
        {
            std::cout << "ERROR - please check your Personalausweis! Exiting Plugin." << std::endl;
            return EXIT_FAILURE;
        }
        else if (outputstring == "e3")
        {
            std::cout << "ERROR - please check your cardreader! Exiting Plugin." << std::endl;
            return EXIT_FAILURE;
        }

        else if (outputstring == "e4")
        {

            //std::cout << "ERROR - AusweisApp2-version less than 1.22.* please update! Exiting Plugin." << std::endl;
            std::cout << "ERROR - AusweisApp2-version less than 1.20.* please update! Exiting Plugin." << std::endl;
            return EXIT_FAILURE;
        }

        else if (outputstring == "e5")
        {
            std::cout << "Warning - retryCounter of Perso <3, please start a selfauthentication direct with AusweisApp2! Exiting Plugin." << std::endl;
            return EXIT_FAILURE;
        }
        else if (outputstring == "e7")
        {
            std::cout << "ERROR - no cardreader found! Exiting Plugin." << std::endl;
            return EXIT_FAILURE;
        }

        //workflowoutputstring="123\n";
        //workflowoutputstring=workflowoutputstring+"\n";

        std::cout << "workflowhash is: " << workflowoutputstring << std::endl;

        std::string myhashkey="";
        bool hashok=false;
        workflowoutputstring.erase(std::remove_if(workflowoutputstring.begin(), workflowoutputstring.end(), isSpace()), workflowoutputstring.end());
        if(hashkey==NULL) perror ("Error opening file hashkey.txt");
        else
        {
           while(fgets(mystring, 100, hashkey)!=NULL)
           {
                std::cout << "hash from file : " << std::string(mystring) << std::endl;
                myhashkey=std::string(mystring);
                myhashkey.erase(std::remove_if(myhashkey.begin(), myhashkey.end(), isSpace()), myhashkey.end());

                if(myhashkey == workflowoutputstring)
                {
                    hashok=true;
                }
            }
         }

        
        if(hashok)
        {
           std::cout << "hashkeys are equal - unlocking door" << std::endl;
           writelog(log, "unlocking door for hashkey "+workflowoutputstring+"\n");
           outputstring = commandexec("python3 locking.py");
           std::cout << outputstring << std::endl;
        }
        else
        {
           std::cout << "hashkeys are not equal - access denied" << std::endl;
           writelog(log, "access denied for hashkey "+workflowoutputstring+"\n");

        }





        //fprintf(phashkey, workflowoutputstring.c_str());

        /*

        fp = popen("diff hashkey.txt producedhashkey.txt", "r");

        //read output a line at a time

        if(fgets(line, sizeof(line), fp)!=NULL)

        {

           std::cout << "diff not equal" << std::endl;

        }

        else 

        {

           std::cout << "diff equal" << std::endl;

        }

        */





        /*

        std::cout << "---------------------------------------" << std::endl;

        //read the output of the parsed jsondata

        std::wcout << "parsed jsonresult as StyledString: \n" << strtowstr(wf.personalStyledString) << std::endl;

        std::cout << "---------------------------------------" << std::endl;

        //read the values of single datanames

        std::wcout << "AcademicTitle: " << strtowstr(wf.AcademicTitle) << std::endl;

        std::wcout << "ArtisticName: " << strtowstr(wf.ArtisticName) << std::endl;

        std::wcout << "BirthName: " << strtowstr(wf.BirthName) << std::endl;

        std::wcout << "DateOfBirth: " << strtowstr(wf.DateOfBirth) << std::endl;

        std::wcout << "DocumentType: " << strtowstr(wf.DocumentType) << std::endl;

        std::wcout << "FamilyNames: " << strtowstr(wf.FamilyNames) << std::endl;

        std::wcout << "GivenNames: " << strtowstr(wf.GivenNames) << std::endl;

        std::wcout << "IssuingState: " << strtowstr(wf.IssuingState) << std::endl;

        std::wcout << "Nationality: " << strtowstr(wf.Nationality) << std::endl;

        //personaldata PlaceOfBirth

        std::wcout << "PlaceOfBirth: " << strtowstr(wf.PlaceOfBirth) << std::endl;

        //personaldata PlaceOfResidence

        std::wcout << "City: " << strtowstr(wf.City) << std::endl;

        std::wcout << "Country: " << strtowstr(wf.Country) << std::endl;

        std::wcout << "Street: " << strtowstr(wf.Street) << std::endl;

        std::wcout << "ZipCode: " << strtowstr(wf.ZipCode) << std::endl;

        std::cout << "---------------------------------------\n" << std::endl;

        std::wcout << "keyhash of workflow: \n" << strtowstr(outputstring) << "\n\n" << std::endl;

        std::cout << "---------------------------------------" << std::endl;

        */

    }


    fclose(hashkey);
    fclose(PINfile);

    fclose(log);

}

