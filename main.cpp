#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

class Student
{
public:
    Student(){};
    Student(string index,int score){
        this->index_no=index;
        this->score=score;
        this->grade= calculateGrade(score);
    }

    string getIndex(){return index_no;}
    int getScore(){return score;}
    char getGrade(){ return grade;}

    void displayMarks(){
        cout<<"\t\t"<<index_no<<"\t"<<score<<"\t"<<grade<<endl;
    }

private:
    string index_no;
    int score;
    char grade;
    char calculateGrade(int score);
};

class Subject;

class Semester{
public:
    void loadDataFromFile(string path);
    void saveSummaries(string path);
    void displaySubject(string subject);
    void displayStudent(string student);
    void displaySubjectSummary(string subject);

private:
    const int MAXSUBJECTS=10;
    const int MAXSTUDENTS=100;
    Subject** subject_data=new Subject*[MAXSUBJECTS];
    int subject_count=0;
};

class Subject {
public:
    Subject(){};
    Subject(string code,int count) {
        this->code = code;
        this->student_count=count;
    };

    string getSubjectName(){return code;};
    Student* getStudent(string index);
    void displaySubject();
    void displaySubjectSummary();
    string getSubjectSummary();

private:
    string code;
    int student_count;
    Student** marks_array;

    double getAverage();
    double getStdDeviation();
    map<char, int> getGradeSummary();
    map<char, double> getGradePercentageSummary();

    friend void Semester::loadDataFromFile(string path); //made a friend so that marks array can be directly accessed
};

int main(int argc, char** argv) {
    Semester semester;
    semester.loadDataFromFile(argv[1]);

    do {
        cout<<endl;
        cout << "Select an option." << endl;
        cout << "\t1.Display Subject" << endl;
        cout << "\t2.Display Student" << endl;
        cout << "\t3.Display Subject Summary" << endl;
        cout << "\t4.Save Summaries" << endl;
        cout << "\t5.Exit Program" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
            case 1: //display Subject
            {
                cout << "Please enter the subject." << endl;
                string subject;
                cin >> subject;
                semester.displaySubject(subject);
                break;
            }
            case 2: //display Student
            {
                cout << "Please enter the student index number." << endl;
                string index;
                cin >> index;
                semester.displayStudent(index);
                break;
            }
            case 3: //display Subject Summary
            {
                cout << "Please enter the subject." << endl;
                string subject;
                cin >> subject;
                semester.displaySubjectSummary(subject);
                break;
            }
            case 4: //save summaries
                semester.saveSummaries("summdata.txt");
                break;
            case 5: //exit the program
                exit(0);
            default:
                cout << "Invalid choice number entered. Try again!" << endl;
        }
        cout<<"Press any key to continue..."<<endl;
        cin.ignore(INT_MAX, '\n');
    }while (cin.get()=='\n');

    return 0;
}

//-------- method definitions--------
char Student::calculateGrade(int score)
{
    if(score<30)
        return 'F';
    if (score<40)
        return 'D';
    if (score<55)
        return 'C';
    if (score<70)
        return 'B';
    return 'A';
}

void Subject::displaySubject() {
    cout<<"\tName:"<<code<<endl;
    cout<<"\tNo. of enrolled students:"<<student_count<<endl;
    cout<<"\tResults:"<<endl;
    cout<<"\t\tStudentID\tMarks\tGrade"<<endl;
    for(int i=0;i<student_count;i++)
    {
        marks_array[i]->displayMarks();
    }
}

void Subject::displaySubjectSummary() {
    cout<<"\tName: "<<code<<endl;
    cout<<"\tNo. of enrolled students: "<<student_count<<endl;
    cout<<"\tMarks Average: "<<getAverage()<<endl;
    cout<<"\tMarks Std Deviation: "<<getStdDeviation()<<endl;
    cout<<"\tGrade Summary:"<<endl;
    cout<<"\t\tGrade\tCount"<<endl;
    map<char ,int> gradeSummaryMap= getGradeSummary();
    for (map<char,int>::iterator it=gradeSummaryMap.begin(); it!=gradeSummaryMap.end(); ++it)
        cout <<"\t\t"<< it->first << "\t" << it->second << '\n';
}

Student* Subject::getStudent(string index){
    for(int i=0;i<student_count;i++)
    {
        if(marks_array[i]->getIndex()==index)
        {
            return marks_array[i];
        }
    }
    return NULL;

}

double Subject::getAverage()
{
    double sum=0;
    for (int i = 0; i < student_count ; i++) {
        sum+=marks_array[i]->getScore();
    }
    return sum/student_count;
}


double Subject::getStdDeviation()
{
    double mean=getAverage();
    double sum=0;
    for (int i = 0; i < student_count ; i++) {
        sum+= pow(marks_array[i]->getScore() - mean , 2);
    }
    return sqrt(sum/student_count);
}

map<char, int> Subject::getGradeSummary()
{
    std::map<char,int> gradeSummaryMap;

    gradeSummaryMap['A']=0,gradeSummaryMap['B']=0,gradeSummaryMap['C']=0;
    gradeSummaryMap['D']=0,gradeSummaryMap['F']=0;

    for (int i = 0; i < student_count ; ++i) {
        gradeSummaryMap[marks_array[i]->getGrade()]++;
    }
    return gradeSummaryMap;
}

map<char, double> Subject::getGradePercentageSummary()
{
    std::map<char,int> gradeSummaryMap=getGradeSummary();
    std::map<char,double> gradeSummaryPerMap;

    gradeSummaryPerMap['A']=0,gradeSummaryPerMap['B']=0,gradeSummaryPerMap['C']=0;
    gradeSummaryPerMap['D']=0,gradeSummaryPerMap['F']=0;

    for (map<char,int>::iterator it=gradeSummaryMap.begin(); it!=gradeSummaryMap.end(); ++it)
        gradeSummaryPerMap[it->first]=(it->second/(1.0*student_count))*100;
    return gradeSummaryPerMap;
}

string Subject::getSubjectSummary() {
    ostringstream oss;
    oss<<code<<" "<<student_count+" ";
    map<char ,double> gradeSummaryMap= getGradePercentageSummary();
    for (map<char,double>::iterator it=gradeSummaryMap.begin(); it!=gradeSummaryMap.end(); ++it)
        oss<<it->first<<" "<<it->second<<"% " ;
    oss<<"\n";
    return oss.str();
}

bool sortByIndex(Student & i,Student & j)
{
    cout<<"Match"<<i.getIndex()<<endl;
    return stoi(i.getIndex()) < stoi(j.getIndex());
}

void Semester::loadDataFromFile(string path)
{
    ifstream file(path);
    if (file.is_open()) {
        string subject_code;
        int student_count;
        while (file>>subject_code>>student_count) {
            if(student_count>MAXSTUDENTS){
                cout<<"Student count exceeded: "<<student_count<<endl;
            }
            subject_count++;
            Subject* subject=new Subject(subject_code,student_count);
            Student** student_array=new Student*[student_count];

            for(int i=0;i<student_count;i++){
                string index; int score;
                file>>index>>score;
                student_array[i] = new Student(index,score);
            }
            sort((*student_array),(*(student_array+student_count)),sortByIndex);
            subject->marks_array=student_array;
            subject_data[subject_count-1]= subject;
        }
        file.close();
    }
    cout << "Data Loading completed..." << endl;
}

void Semester::displaySubject(string subject)
{
    bool found= false;
    for(int i=0;i<subject_count;i++)
    {
        if(subject_data[i]->getSubjectName()==subject)
        {
            subject_data[i]->displaySubject();
            found=true;
            break;
        }
    }
    if(!found)
        cout<<"ERROR: No Data found on given subject"<<endl;
}

void Semester::displayStudent(string student)
{
    bool found= false;
    cout<<"\tResults:"<<endl;
    cout<<"\tSubject\t\tStudentID\tMarks\tGrade"<<endl;
    for(int i=0;i<subject_count;i++)
    {
        Student* marks=subject_data[i]->getStudent(student);

        if(marks!= NULL)
        {
            cout<<"\t"<<subject_data[i]->getSubjectName()<<"";
            marks->displayMarks();
            found=true;
        }
    }
    if(!found)
        cout<<"ERROR: No Data found on given student"<<endl;
}

void Semester::displaySubjectSummary(string subject)
{
    bool found= false;
    for(int i=0;i<subject_count;i++)
    {
        if(subject_data[i]->getSubjectName()==subject)
        {
            subject_data[i]->displaySubjectSummary();
            found=true;
            break;
        }
    }
    if(!found)
        cout<<"ERROR: No Data found on given subject"<<endl;
}

void Semester::saveSummaries(string path) {
    ofstream outfile (path);
    if (outfile.is_open())
    {
        for(int i=0;i<subject_count;i++)
        {
            outfile<<subject_data[i]->getSubjectSummary();
        }
        outfile.close();
    }
    cout<<"Written Subjects Summary to file Successfully"<<endl;
}