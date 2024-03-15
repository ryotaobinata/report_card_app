#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<filesystem>
#include<limits>

using namespace std;

const int mode_min = 0;
const int mode_max = 3;


struct ReportCard
{
	int grade;
	string term;
	string subject;
	string result="-";
};

void add_subject(vector<ReportCard>& report_card);
void show_report_card(const vector<ReportCard>& report_card);
void get_report_card(string file_name, vector<ReportCard>& report_card);
void save_report_card(string file_name, vector<ReportCard>& report_card);
void remove_report_card(vector<ReportCard>& report_card);
void add_result(vector<ReportCard>& report_card);
void add_mode(vector<ReportCard>& report_card);
void sort_report_card(vector<ReportCard>& report_card);
void show_mode(const vector<ReportCard>& report_card, vector<ReportCard>& filtering_card);
void show_filter_report(const vector<ReportCard>& report_card, vector<ReportCard>& filtering_card);
int set_mode();

/*
ファイルの仕様は[ 学期,科目名,成績,学年 ]の順で並んでおり、
4列、複数行のcsvファイルとなっている。
成績は[ 不可,可,良,優,秀 ]の5段階評価である。
例)
  前期,線形代数3,優,2
  後期,ネットワーク2,秀,2
  前期,情報概論,良,1
  前期,確率統計,可,2

*/

int main() {
	
	int mode;
	string file_name = "report_card.csv";
	vector<ReportCard> report_card;
	vector<ReportCard> filtering_card;

	get_report_card(file_name, report_card);

	//モード選択

	do
	{
		mode = set_mode();

		switch (mode)
		{
		case 0:
			add_mode(report_card);
			break;
		case 1:
			sort_report_card(report_card);
			show_mode(report_card, filtering_card);
			break;
		case 2:
			remove_report_card(report_card);
			break;
		case 3:
			save_report_card(file_name, report_card);
			exit(EXIT_SUCCESS);
			break;
		default:
			break;
		}

	} while (true);
	
	
	return 0;
}

int set_mode()
{
	int mode;
	string prompt = "モードを選択してください[ 授業,成績登録:0,成績表:1,授業の削除:2,保存して終了:3 ]＞";
	do
	{
		cout << prompt;
		while (!(cin>>mode))
		{
			cerr << "入力値が不正です" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << prompt;
		}

		if (mode>=mode_min&&mode<=mode_max)
		{
			return mode;
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}

	} while (1);
}

void add_subject(vector<ReportCard>& report_card)
{
	int grade;
	string term;
	string subject;

	cout << "授業の登録をします" << endl;
	
	//学年の入力
	do
	{
		cout << "学年[1～4]＞";
		//入力値チェック
		while (!(cin >> grade))
		{
			cerr << "入力値が不正です" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "学年[1～4]＞";
		}

		if (grade > 0 && grade < 5)
		{
			//ストリームの読み捨て
			cin.ignore();
			break;
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}

	} while (1);
	
	//学期の入力
	do
	{
		cout << "学期[前期/後期]＞";
		getline(cin, term);
		if (term=="前期"||term=="後期")
		{
			break;
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}
	} while (1);

	cout << "科目名＞";
	getline(cin, subject);

	//授業の登録
	report_card.push_back({ grade,term,subject });
	cout << "授業を登録しました" << endl;
}

void add_result(vector<ReportCard>& report_card)
{
	int add_result_num;
	
	do
	{
		show_report_card(report_card);
		cout << "成績を登録する授業の番号を選択してください[0で終了]＞";

		//入力値チェック
		//整数でない場合
		while (!(cin >> add_result_num))
		{
			cerr << "入力値が不正です" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			show_report_card(report_card);
			cout << "成績を登録する授業の番号を選択してください[0で終了]＞";
		}

		if (report_card.size() < add_result_num || add_result_num < 0)
		{
			cerr << "入力値が不正です" << endl;
			continue;
		}

		if (add_result_num == 0) {
			cout << "登録を終了します" << endl;
			break;
		}

		//成績の追加
		do
		{
			string result;
			cout << "成績を入力してください[不可,可,良,優,秀]＞";
			cin >> result;
			//ストリームの読み捨て
			cin.ignore();
			if (result=="不可"||result=="可"||result=="良"||result=="優" ||result=="秀")
			{
				//成績の登録
				report_card.at(add_result_num-1).result=result;
				break;
			}
			else
			{
				cerr << "入力値が不正です" << endl;
			}
		} while (1);
		
	} while (1);
}

void show_report_card(const vector<ReportCard>& report_card)
{
	int count = 1;
	
	cout << "現在登録されている授業" << endl
		<< "   学年 , 学期 , 科目名 , 成績" << endl;

	for (const auto& report : report_card)
	{
		cout << setw(3) << count << ": ";
		cout << report.grade << " , " << report.term << " , " << report.subject << " , " << report.result << endl;
		count++;
	}
}

void get_report_card(string file_name, vector<ReportCard>& report_card)
{
	ifstream ifs;
	if (filesystem::exists(file_name))
	{
		//ファイルのオープン
		ifs.open(file_name);
		if (!ifs)
		{
			cerr << "ファイルのオープンに失敗しました";
			exit(EXIT_FAILURE);
		}

		//ファイルから一行ずつ読み取る
		string line;
		while (getline(ifs, line)) {
			
			istringstream iss(line);
			int grade;
			string term;
			string subject;
			string reslut;
			string checkbuff;

			if (getline(iss,term,',')&&getline(iss,subject,',')&&getline(iss,reslut,',')&&(iss>>grade))
			{
				if (getline(iss,checkbuff,','))
				{
					cerr << "ファイルの仕様が異なります";
					exit(EXIT_FAILURE);
				}

				//読み取った結果を格納
				report_card.push_back({ grade,term,subject,reslut });
			}
			else
			{
				cerr << "ファイルの仕様が異なります";
				exit(EXIT_FAILURE);
			}
		}
	}
	
}

void save_report_card(string file_name, vector<ReportCard>& report_card)
{
	ofstream ofs;
	ofs.open(file_name);
	if (!ofs)
	{
		cerr << "ファイルのオープンに失敗しました";
		exit(EXIT_FAILURE);
	}

	for (const auto& report : report_card)
	{
		ofs << report.term << ',' << report.subject << ',' << report.result << ',' << report.grade << endl;
	}

	ofs.close();
	cout << "ファイルへ保存しました";

}

void remove_report_card(vector<ReportCard>& report_card)
{
	int remove_num;

	do
	{
		show_report_card(report_card);
		cout << "削除する授業の番号を入力してください[0で終了]＞";

		//入力値チェック
		//整数出ない場合
		while (!(cin >> remove_num))
		{
			cerr << "入力値が不正です" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			show_report_card(report_card);
			cout << "削除する授業の番号を入力してください[0で終了]＞";
		}

		if (report_card.size() < remove_num||remove_num<0)
		{
			cerr << "入力値が不正です" << endl;
			continue;
		}

		if (remove_num == 0) {
			cout << "削除を終了します" << endl;
			break;
		}

		//授業の削除
		report_card.erase(report_card.begin() + (remove_num - 1));
		cout << "授業 " << remove_num << "を削除しました" << endl;
	} while (1);
	
}

void add_mode(vector<ReportCard>& report_card)
{
	int mode;
	string prompt="どちらを登録しますか[授業:1,成績:2,終了:0]＞";
	do
	{
		cout << prompt;
		while (!(cin >> mode))
		{
			cerr << "入力値が不正です" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << prompt;
		}

		if (mode >= 0 && mode <= 2)
		{
			switch (mode)
			{
			case 0:
				return;
			case 1:
				add_subject(report_card);
				break;
			case 2:
				add_result(report_card);
				break;
			default:
				break;
			}
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}

	} while (1);
}

void sort_report_card(vector<ReportCard>& report_card)
{
	sort(report_card.begin(), report_card.end(), [](const ReportCard& a, const ReportCard& b) {
			return a.grade < b.grade;
		});
}

void show_mode(const vector<ReportCard>& report_card, vector<ReportCard>& filtering_card)
{
	int mode;
	string prompt = "表示方法を選んでください[そのまま:1,フィルタリング:2,終了:0]＞";

	do
	{
		cout << prompt;
		while (!(cin >> mode))
		{
			cerr << "入力値が不正です" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << prompt;
		}

		if (mode >= 0 && mode <= 2)
		{
			switch (mode)
			{
			case 0:
				return;
			case 1:
				show_report_card(report_card);
				break;
			case 2:
				show_filter_report(report_card, filtering_card);
				break;
			default:
				break;
			}
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}

	} while (1);
}

void show_filter_report(const vector<ReportCard>& report_card, vector<ReportCard>& filtering_card)
{
	int count = 1;
	int grade;
	string term;
	string result;

	filtering_card.clear();
	filtering_card.resize(report_card.size());

	cout << "フィルターを設定してください[指定しない場合は0を入力]" << endl;

	do
	{
		cout << "学年[1～4]＞";
		//入力値チェック
		while (!(cin >> grade))
		{
			cerr << "入力値が不正です" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "学年[1～4]＞";
		}

		if (grade >= 0 && grade < 5)
		{
			//ストリームの読み捨て
			cin.ignore();
			break;
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}

	} while (1);

	//学期の入力
	do
	{
		cout << "学期[前期/後期]＞";
		getline(cin, term);
		if (term == "前期" || term == "後期"||term=="0")
		{
			
			break;
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}
	} while (1);
	

	//成績の追加
	do
	{
		cout << "成績を入力してください[不可,可,良,優,秀]＞";
		getline(cin, result);
	
		if (result == "不可" || result == "可" || result == "良" || result == "優" || result == "秀"||result=="0")
		{	
			break;
		}
		else
		{
			cerr << "入力値が不正です" << endl;
		}
	} while (1);

	

	auto end=copy_if(report_card.begin(), report_card.end(), filtering_card.begin(), [grade, term, result](const ReportCard& a) {
		if (grade == 0)
		{
			if (term == "0")
			{
				if (result == "0") return true;
				else return (a.result == result);
			}
			else
			{
				if (result == "0") return (a.term == term);
				else return(a.term==term&&a.result==result);
			}
		}
		else
		{
			if (term=="0")
			{
				if (result == "0")return (a.grade == grade);
				else return (a.grade == grade && a.result == result);
			}
			else
			{
				if (result == "0") return(a.grade == grade && a.term == term);
				else return (a.grade == grade && a.term == term && a.result == result);
			}
		}
			
		});
	

	cout << "   学年 , 学期 , 科目名 , 成績" << endl;

	
	for (const auto& filter : filtering_card)
	{
		if (filter.grade==0)
		{
			break;
		}
		cout << setw(3) << count << ": ";
		cout << filter.grade << " , " << filter.term << " , " << filter.subject << " , " << filter.result << endl;
		count++;
	}
}