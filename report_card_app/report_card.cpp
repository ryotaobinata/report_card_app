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
�t�@�C���̎d�l��[ �w��,�Ȗږ�,����,�w�N ]�̏��ŕ���ł���A
4��A�����s��csv�t�@�C���ƂȂ��Ă���B
���т�[ �s��,��,��,�D,�G ]��5�i�K�]���ł���B
��)
  �O��,���`�㐔3,�D,2
  ���,�l�b�g���[�N2,�G,2
  �O��,���T�_,��,1
  �O��,�m�����v,��,2

*/

int main() {
	
	int mode;
	string file_name = "report_card.csv";
	vector<ReportCard> report_card;
	vector<ReportCard> filtering_card;

	get_report_card(file_name, report_card);

	//���[�h�I��

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
	string prompt = "���[�h��I�����Ă�������[ ����,���ѓo�^:0,���ѕ\:1,���Ƃ̍폜:2,�ۑ����ďI��:3 ]��";
	do
	{
		cout << prompt;
		while (!(cin>>mode))
		{
			cerr << "���͒l���s���ł�" << endl;
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
			cerr << "���͒l���s���ł�" << endl;
		}

	} while (1);
}

void add_subject(vector<ReportCard>& report_card)
{
	int grade;
	string term;
	string subject;

	cout << "���Ƃ̓o�^�����܂�" << endl;
	
	//�w�N�̓���
	do
	{
		cout << "�w�N[1�`4]��";
		//���͒l�`�F�b�N
		while (!(cin >> grade))
		{
			cerr << "���͒l���s���ł�" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�w�N[1�`4]��";
		}

		if (grade > 0 && grade < 5)
		{
			//�X�g���[���̓ǂݎ̂�
			cin.ignore();
			break;
		}
		else
		{
			cerr << "���͒l���s���ł�" << endl;
		}

	} while (1);
	
	//�w���̓���
	do
	{
		cout << "�w��[�O��/���]��";
		getline(cin, term);
		if (term=="�O��"||term=="���")
		{
			break;
		}
		else
		{
			cerr << "���͒l���s���ł�" << endl;
		}
	} while (1);

	cout << "�Ȗږ���";
	getline(cin, subject);

	//���Ƃ̓o�^
	report_card.push_back({ grade,term,subject });
	cout << "���Ƃ�o�^���܂���" << endl;
}

void add_result(vector<ReportCard>& report_card)
{
	int add_result_num;
	
	do
	{
		show_report_card(report_card);
		cout << "���т�o�^������Ƃ̔ԍ���I�����Ă�������[0�ŏI��]��";

		//���͒l�`�F�b�N
		//�����łȂ��ꍇ
		while (!(cin >> add_result_num))
		{
			cerr << "���͒l���s���ł�" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			show_report_card(report_card);
			cout << "���т�o�^������Ƃ̔ԍ���I�����Ă�������[0�ŏI��]��";
		}

		if (report_card.size() < add_result_num || add_result_num < 0)
		{
			cerr << "���͒l���s���ł�" << endl;
			continue;
		}

		if (add_result_num == 0) {
			cout << "�o�^���I�����܂�" << endl;
			break;
		}

		//���т̒ǉ�
		do
		{
			string result;
			cout << "���т���͂��Ă�������[�s��,��,��,�D,�G]��";
			cin >> result;
			//�X�g���[���̓ǂݎ̂�
			cin.ignore();
			if (result=="�s��"||result=="��"||result=="��"||result=="�D" ||result=="�G")
			{
				//���т̓o�^
				report_card.at(add_result_num-1).result=result;
				break;
			}
			else
			{
				cerr << "���͒l���s���ł�" << endl;
			}
		} while (1);
		
	} while (1);
}

void show_report_card(const vector<ReportCard>& report_card)
{
	int count = 1;
	
	cout << "���ݓo�^����Ă������" << endl
		<< "   �w�N , �w�� , �Ȗږ� , ����" << endl;

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
		//�t�@�C���̃I�[�v��
		ifs.open(file_name);
		if (!ifs)
		{
			cerr << "�t�@�C���̃I�[�v���Ɏ��s���܂���";
			exit(EXIT_FAILURE);
		}

		//�t�@�C�������s���ǂݎ��
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
					cerr << "�t�@�C���̎d�l���قȂ�܂�";
					exit(EXIT_FAILURE);
				}

				//�ǂݎ�������ʂ��i�[
				report_card.push_back({ grade,term,subject,reslut });
			}
			else
			{
				cerr << "�t�@�C���̎d�l���قȂ�܂�";
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
		cerr << "�t�@�C���̃I�[�v���Ɏ��s���܂���";
		exit(EXIT_FAILURE);
	}

	for (const auto& report : report_card)
	{
		ofs << report.term << ',' << report.subject << ',' << report.result << ',' << report.grade << endl;
	}

	ofs.close();
	cout << "�t�@�C���֕ۑ����܂���";

}

void remove_report_card(vector<ReportCard>& report_card)
{
	int remove_num;

	do
	{
		show_report_card(report_card);
		cout << "�폜������Ƃ̔ԍ�����͂��Ă�������[0�ŏI��]��";

		//���͒l�`�F�b�N
		//�����o�Ȃ��ꍇ
		while (!(cin >> remove_num))
		{
			cerr << "���͒l���s���ł�" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			show_report_card(report_card);
			cout << "�폜������Ƃ̔ԍ�����͂��Ă�������[0�ŏI��]��";
		}

		if (report_card.size() < remove_num||remove_num<0)
		{
			cerr << "���͒l���s���ł�" << endl;
			continue;
		}

		if (remove_num == 0) {
			cout << "�폜���I�����܂�" << endl;
			break;
		}

		//���Ƃ̍폜
		report_card.erase(report_card.begin() + (remove_num - 1));
		cout << "���� " << remove_num << "���폜���܂���" << endl;
	} while (1);
	
}

void add_mode(vector<ReportCard>& report_card)
{
	int mode;
	string prompt="�ǂ����o�^���܂���[����:1,����:2,�I��:0]��";
	do
	{
		cout << prompt;
		while (!(cin >> mode))
		{
			cerr << "���͒l���s���ł�" << endl;
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
			cerr << "���͒l���s���ł�" << endl;
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
	string prompt = "�\�����@��I��ł�������[���̂܂�:1,�t�B���^�����O:2,�I��:0]��";

	do
	{
		cout << prompt;
		while (!(cin >> mode))
		{
			cerr << "���͒l���s���ł�" << endl;
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
			cerr << "���͒l���s���ł�" << endl;
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

	cout << "�t�B���^�[��ݒ肵�Ă�������[�w�肵�Ȃ��ꍇ��0�����]" << endl;

	do
	{
		cout << "�w�N[1�`4]��";
		//���͒l�`�F�b�N
		while (!(cin >> grade))
		{
			cerr << "���͒l���s���ł�" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�w�N[1�`4]��";
		}

		if (grade >= 0 && grade < 5)
		{
			//�X�g���[���̓ǂݎ̂�
			cin.ignore();
			break;
		}
		else
		{
			cerr << "���͒l���s���ł�" << endl;
		}

	} while (1);

	//�w���̓���
	do
	{
		cout << "�w��[�O��/���]��";
		getline(cin, term);
		if (term == "�O��" || term == "���"||term=="0")
		{
			
			break;
		}
		else
		{
			cerr << "���͒l���s���ł�" << endl;
		}
	} while (1);
	

	//���т̒ǉ�
	do
	{
		cout << "���т���͂��Ă�������[�s��,��,��,�D,�G]��";
		getline(cin, result);
	
		if (result == "�s��" || result == "��" || result == "��" || result == "�D" || result == "�G"||result=="0")
		{	
			break;
		}
		else
		{
			cerr << "���͒l���s���ł�" << endl;
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
	

	cout << "   �w�N , �w�� , �Ȗږ� , ����" << endl;

	
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