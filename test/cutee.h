/***************************************************************************
    copyright            : (C) 2003-2005 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: cutee.h,v 1.14 2008-10-07 15:43:55 tat Exp $
 ***************************************************************************/
#ifndef _CUTEE_CUTEE_H_
#define _CUTEE_CUTEE_H_
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdarg.h>

typedef unsigned int uint;

#define CUTEE_VERSION "0.4.2"

#define MAX_TEST_COUNT 1000


#define TEST_CLASS( name ) \
    dummy_##name: public cutee::CuteeTest { \
    public: \
        dummy_##name() \
        { \
            className(#name); \
            classFileName(__FILE__); \
            classLineNo(__LINE__); \
        } \
    }; \
    struct name: public dummy_##name
#define TEST_FUNCTION_EX( funcname, filename, lineno ) \
    run_##funcname() \
    { \
        functionName(#funcname); \
        functionLineNo(lineno); \
        mEvt->enterFunction(*this); \
        mFuncExitCode = 0; \
        funcname(); \
        if(mFuncExitCode) mFailed++; \
        mEvt->leaveFunction(*this, mFuncExitCode == 0); \
    } \
    void funcname()


#define TEST_FUNCTION( funcname ) \
    TEST_FUNCTION_EX( funcname, __FILE__, __LINE__)

#define PRINT_VALUE( a ) \
    "\t[" << #a << ": " << a << "]" << std::endl

#define PRINT_ON_FAILURE_1( a ) \
    PRINT_VALUE( a ) 
#define PRINT_ON_FAILURE_2( a, b ) \
    PRINT_VALUE( a ) << PRINT_VALUE( b )
#define PRINT_ON_FAILURE_3( a, b, c ) \
    PRINT_VALUE( a ) << PRINT_VALUE( b ) << PRINT_VALUE( c )
#define PRINT_ON_FAILURE_4( a, b, c, d ) \
    PRINT_VALUE( a ) << PRINT_VALUE( b ) << \
    PRINT_VALUE( c ) << PRINT_VALUE( d )

#define TEST_ASSERT_EX( e, file, line ) \
    do { expr(#e); exprFileName(file); exprLineNo(line); \
    testAssert(e); } while(0)

#define TEST_ASSERT_EX_M( e, p, file, line ) \
    do { expr(#e); exprFileName(file); exprLineNo(line); \
    std::stringstream _l_ss; _l_ss << p << std::endl; \
    testAssertM(e, _l_ss ); } while(0) 



#define TEST_ASSERT( expr ) TEST_ASSERT_EX( expr, __FILE__, __LINE__ ) 
#define TEST_ASSERT_EQUALS( a, b ) TEST_ASSERT_EX( (a == b), __FILE__,__LINE__ )
#define TEST_ASSERT_DIFFERS( a, b ) TEST_ASSERT_EX( (a != b),__FILE__,__LINE__ )

// print p on test fail
#define TEST_ASSERT_M( expr , p) TEST_ASSERT_EX_M( expr, p, __FILE__,__LINE__ ) 
#define TEST_ASSERT_EQUALS_M( a, b, p ) TEST_ASSERT_EX_M( (a == b), p,__FILE__,__LINE__ )
#define TEST_ASSERT_DIFFERS_M( a, b, p ) TEST_ASSERT_EX_M( (a != b), p,__FILE__,__LINE__)

// print a and b on error
#define TEST_ASSERT_P( expr ) TEST_ASSERT_EX_M( expr, PRINT_ON_FAILURE_1( expr ), __FILE__,__LINE__ ) 
#define TEST_ASSERT_EQUALS_P( a, b ) TEST_ASSERT_EX_M( (a == b), PRINT_ON_FAILURE_2( a, b ), __FILE__,__LINE__ )
#define TEST_ASSERT_DIFFERS_P( a, b ) TEST_ASSERT_EX_M( (a != b), PRINT_ON_FAILURE_2( a, b ), __FILE__,__LINE__)

#define TEST_ASSERT_PM( expr, m ) TEST_ASSERT_EX_M( expr, PRINT_ON_FAILURE_2( expr, m ), __FILE__,__LINE__ ) 
#define TEST_ASSERT_EQUALS_PM( a, b, m ) TEST_ASSERT_EX_M( (a == b), PRINT_ON_FAILURE_3( a, b, m ), __FILE__,__LINE__ )
#define TEST_ASSERT_DIFFERS_PM( a, b, m ) TEST_ASSERT_EX_M( (a != b), PRINT_ON_FAILURE_3( a, b, m ), __FILE__,__LINE__)

#define TEST_ASSERT_OR_RET(expr, msg) \
    do { if(!expr) { TEST_ASSERT_M(expr, msg); return; } } while(0)


namespace cutee // c++ unit testing environment 
{

struct CuteeTest;

// store runtime context (class name, function name, line numbers, etc.)
struct Context
{
    typedef std::string string;
    Context()
    : mClassLineNo(0), mFunctionLineNo(0), mExprLineNo(0) 
    {}
    // class info
    const string& className() const    { return mClassName; }
    uint classLineNo() const { return mClassLineNo; }
    const string& classFileName() const { return mClassFileName; }
    // filename
    const string& fileName() const { return mFileName; }
    // function info
    const string& functionName() const { return mFunctionName; }
    uint functionLineNo() const { return mFunctionLineNo; }
    // assertion info
    const string& expr() const { return mExpr; }
    const string& exprFileName() const { return mExprFileName; }
    uint exprLineNo() const { return mExprLineNo; }

    void className(const string& s) { mClassName = s; }
    void classFileName(const string& s) { mClassFileName = s; }
    void classLineNo(uint i) { mClassLineNo = i; }

    void filename(const string& s) { mFileName = s; }

    void functionName(const string& s) { mFunctionName = s; }
    void functionLineNo(uint i) { mFunctionLineNo = i; }

    void expr(const string& s) { mExpr = s; }
    void exprFileName(const string& s) { mExprFileName = s; }
    void exprLineNo(uint i) { mExprLineNo = i; }
private:
    std::string mClassName, mClassFileName, mFunctionName, 
        mFileName, mExpr, mExprFileName;
    uint mClassLineNo, mFunctionLineNo, mExprLineNo;
};


// execution monitor interface
struct TestRunMonitor
{
    virtual ~TestRunMonitor() {}
    
    virtual void beginTesting() {}
    virtual void endTesting() {}

    // virtual void enterSuite(const CuteeTest&) {}
    // virtual void leaveSuite(const CuteeTest&, int b) {}

    virtual void enterClass(const CuteeTest&) {}
    virtual void leaveClass(const CuteeTest&, int) {}

    virtual void enterFunction(const CuteeTest&) {}
    virtual void leaveFunction(const CuteeTest&, int ) {}

    virtual void assertion(const CuteeTest&, int, 
        const std::string& u = "") {}
};

// statically store pointers to test classes
struct TestList
{
    virtual ~TestList() {}
    // FIXME add dynamic array alloc to avoid fixed limit
    static CuteeTest* list[MAX_TEST_COUNT];
    static int list_idx;
};



// this is the base class of all cutee test classes
struct CuteeTest: public cutee::Context 
{
    CuteeTest()
    : mEvt(&mNullMonitor), mFuncExitCode(0), mFailed(0)
    {
    }
    virtual ~CuteeTest() {}
    virtual void setUp() {}
    virtual void tearDown()    {}
    void testRunMonitor(TestRunMonitor *evt) { mEvt = evt; }
    int passed() const  { return mFailed == 0; }
    virtual void run() = 0;
    virtual uint count() = 0;
protected:
    void testAssert(int b)
    {
        if( b == 0 ) // assertion failed 
            mFuncExitCode++; 
        mEvt->assertion(*this, b);
    }
    void testAssertM(int b, std::stringstream& os)
    {
        if( b == 0 ) // assertion failed 
            mFuncExitCode++; 
        mEvt->assertion(*this, b, os.str());
    }
protected:
    TestRunMonitor *mEvt, mNullMonitor;
    uint mFuncExitCode, mFailed;
};



// monitors execution and keep statistics on classes/functions/tests
struct StatsMonitor: public TestRunMonitor
{
    StatsMonitor()
    : mAssertPassed(0), mAssertFailed(0), mAssertCount(0),
      mFuncPassed(0),  mFuncFailed(0), mFuncCount(0),
      mClassPassed(0),  mClassFailed(0), mClassCount(0)
    {
    }
    // void enterSuite(const CuteeTest& t)    {}
    // void leaveSuite(const CuteeTest& t, int b) {}
    void enterClass(const CuteeTest& t) 
    {
        mClassCount++;
    }
    void leaveClass(const CuteeTest& t, int b) 
    {
        if(b) 
            mClassPassed++;
        else 
            mClassFailed++;
    }
    void enterFunction(const CuteeTest&)
    {
        mFuncCount++;
    }
    void leaveFunction(const CuteeTest&, int b) 
    {
        if(b) 
            mFuncPassed++;
        else 
            mFuncFailed++;
    }
    void assertion(const CuteeTest& t, int b, const std::string& userMsg) 
    {
        mAssertCount++;
        if(b) 
            mAssertPassed++;
        else 
            mAssertFailed++;
    }
protected:
    uint mAssertPassed, mAssertFailed, mAssertCount, mFuncPassed,
         mFuncFailed, mFuncCount, mClassPassed, mClassFailed, mClassCount;
};

// keep stats and print results on exit
struct ConsoleRunMonitor: public StatsMonitor
{
    void assertion(const CuteeTest& t, int b, const std::string& userMsg) 
    {
        using namespace std;
        StatsMonitor::assertion(t, b, userMsg);
        if(!b) 
        {
            cout << endl << " [" 
                << t.exprFileName() << ":" 
                << t.exprLineNo()<< "] "
                << t.className() << "::" 
                << t.functionName() << "(): "
                << t.expr() << " assertion failed" 
                << endl;
            if(!userMsg.empty())
                cout << userMsg << endl;
        }
    }
    void endTesting()
    {
        using namespace std;
        cout << endl << endl;
        cout << "    ======================================" << endl;
        cout << "          Tests Statistics           " << endl;
        cout << "    --------------------------------------" << endl;
        cout << "                Functions       Checks   " << endl;
        cout << "      Success   "<< setw(8) << mFuncPassed << 
            "      " << setw(8) << mAssertPassed << endl;
        cout << "       Failed   "<< setw(8) << mFuncFailed << 
            "      " << setw(8) << mAssertFailed << endl;
        cout << "    --------------------------------------" << endl;
        cout << "        Total   "<< setw(8) << mFuncCount << 
            "      " <<  setw(8) << mAssertCount << endl;
        cout << "    ======================================" << endl;
    }
};

// keep stats and print skimmer xml output on exit
struct SkimmerRunMonitor: public StatsMonitor
{
    void beginTesting() 
    {
        using namespace std;
        cout << "<?xml version=\"1.0\"?>" << endl;
        cout << "<tests>" << endl;
    }
    void endTesting() 
    {
        using namespace std;
        cout << "</tests>" << endl;
    }
    void enterClass(const CuteeTest&)
    {
    }
    void leaveClass(const CuteeTest&, int) 
    {
    }
    void enterFunction(const CuteeTest& t) 
    {
        using namespace std;
        cout << "<test>" << endl;
        cout << "<name>"; 
        cout << t.className() << "::" << t.functionName() << "()";
        cout << "</name>" << endl;
    }
    void leaveFunction(const CuteeTest& t, int b) 
    {
        using namespace std;
        cout << "<type>" << (b ? "pass" : "fail") << "</type>" << endl;
        cout << "</test>" << endl << endl << endl;
    }
    void assertion(const CuteeTest& t, int b, const std::string& u = "") 
    {
        using namespace std;
        if(b)
            return;
        cout << "  <info>" << endl;
        cout << "  <type>" << (b ? "pass" : "fail") << "</type>" << endl;
        cout << "  <data>" << endl;
        cout << "  <file>" << t.exprFileName() << "</file>" << endl;
        cout << "  <line>" << t.exprLineNo() << "</line>" << endl;
        cout << "  <expression>" << t.expr() << "</expression>" <<endl;
        if(!u.empty())
            cout << "  <output>" << u << endl << "  </output>" << endl;
        cout << "  </expression>" << endl;
        cout << "  </data>" << endl;
        cout << "  </info>" << endl << endl;
    }
};

// create the appropriate TestRunMonitor based on environment variables
// or command line parameters and run tests
struct Runner
{
    Runner(int argc, char** argv)
    : mEvt(0)
    {
        if(getenv("SKIMMER_MODE"))
            mEvt = new SkimmerRunMonitor;
        else
            mEvt = new ConsoleRunMonitor;
    }
    void run()
    {
        mEvt->beginTesting();
        for(int i =0; i < TestList::list_idx; i++)
        {
            CuteeTest *ct = TestList::list[i];
            ct->testRunMonitor(mEvt);
            mEvt->enterClass(*ct);
            ct->run();
            mEvt->leaveClass(*ct, ct->passed());
        }
        mEvt->endTesting();
    }
private:
    TestRunMonitor *mEvt;
};

}


#endif
