#include <iostream>
#include <iosfwd>
#include <fstream>

#include "as/io.hpp"

using namespace std;

namespace as
{
    void IOTest()
    {
        std::cout << "Module 'IO' is Ready" << std::endl;
        return;
    }

    void IniFile::SetIniFilePath(const string& filepath)
	{
		m_FilePath = filepath;
		if (m_FilePath.find('.') == -1)
			m_FilePath += ".ini";
	}

	string IniFile::ReadIniContent(const string& section, const string& item, string value)
	{
		ifstream infile;
		infile.open(m_FilePath);
		if (!infile.is_open())
		{
			return value;
		}

		string strline;
		while (!infile.eof())
		{
			getline(infile, strline);
			if (strline == "" || strline.find('[') == -1)
				continue;

			strline = strline.substr(strline.find('[') + 1, strline.find(']') - strline.find('[') - 1);
			if (strline == section)
			{
				while (!infile.eof())
				{
					getline(infile, strline);
					if (strline != "" && strline.find('[') != -1 && strline.find('%') == -1)
						break;
					int equalsignpos = strline.find('='), commentpos = strline.find(';');

					string itemkey = strline.substr(0, equalsignpos);
					TrimBoundary(itemkey);
					if (itemkey != item)
						continue;


					if (commentpos != -1)
						value = strline.substr(equalsignpos + 1, commentpos - equalsignpos - 1);
					else
						value = strline.substr(equalsignpos + 1);

					TrimBoundary(value);
					infile.close();
					return value;
				}
			}
		}
		infile.close();
		return value;
	}


	bool IniFile::WriteIniContent(const string& section, const string& item, const string& value)
	{
		ifstream infile;
		infile.open(m_FilePath);
		if (!infile.is_open())
			return false;

		string strline, writecontent;
		bool finditem = false;
		while (!infile.eof())
		{
			getline(infile, strline);
			writecontent += strline + "\n";

			if (strline == "" || strline.front() != '[')
				continue;

			strline = strline.substr(strline.find('[') + 1, strline.find(']') - strline.find('[') - 1);
			if (strline == section)
			{
				while (!infile.eof())
				{
					getline(infile, strline);
					if (strline != "" && strline.front() == '[')
					{
						finditem = true;
						while (*(writecontent.end() - 2) == '\n')
							writecontent.pop_back();
						writecontent += item + " = " + value + "\n\n";
						writecontent += strline + "\n";
						break;
					}

					writecontent += strline + "\n";

					int equalsignpos = strline.find('=');
					string itemkey = strline.substr(0, equalsignpos);

					TrimBoundary(itemkey);
					if (itemkey == item)
					{
						finditem = true;
						writecontent = writecontent.substr(0, writecontent.size() - strline.size() +
							equalsignpos) + " " + value + "\n";
						break;
					}
				}
			}
		}
		infile.close();

		if (!finditem)
		{
			while (writecontent.size() != 0 && writecontent.back() == '\n')
				writecontent.pop_back();

			writecontent += "\n\n[" + section + "]" + "\n" + item + " =" + value + "\n";
		}
		ofstream outfile;
		outfile.open(m_FilePath);
		if (!outfile.is_open())
			return false;
		while (*(writecontent.end() - 2) == '\n')
			writecontent.pop_back();
		outfile << writecontent;
		outfile.close();

		return true;
	}


	bool IniFile::WriteLightIniContent(map<string, map<string, vector<string>>>iniContent, bool isSearch)
	{
		ofstream outfile;
		outfile.open(m_FilePath);
		if (!outfile.is_open())
			return false;
		string strline, writecontent;
		outfile << "------------------------------- \n";
		if (isSearch)
		{
			outfile << ";亮度校正搜索配置文件：\n";
		}
		else
		{
			outfile << ";亮度校正配置文件：\n";
		}

		outfile << "------------------------------- \n";
		map<string, map<string, vector<string>>>::iterator iter = iniContent.begin();
		while (iter != iniContent.end())
		{
			strline = '[' + iter->first + ']' + "\n";
			outfile << strline;
			map<string, vector<string>>::iterator it = iter->second.begin();

			while (it != iter->second.end())
			{
				vector<string>::iterator vec_it = it->second.begin();
				int index = 0;
				while (vec_it != it->second.end())
				{
					if (index != it->second.size() - 1)
					{
						writecontent = it->first + std::to_string(index) + ' ' + '=' + ' ' + it->second.at(index) + '\n';

					}
					else
					{
						if (isSearch)
						{
							writecontent = it->first + std::to_string(index) + ' ' + '=' + ' ' + it->second.at(index) + '\n' + '\n' + '\n';
						}
						else
						{
							writecontent = it->first + ' ' + '=' + ' ' + it->second.at(index) + '\n';
						}

					}
					outfile << writecontent;
					index++;
					vec_it++;
				}
				it++;
			}
			outfile << '\n';
			iter++;
		}
		outfile.close();

		return true;
	}

	void IniFile::TrimLeft(string& str)
	{
		while (str.size() > 0 && str.front() == ' ')
			str = str.substr(1);
	}

	void IniFile::TrimRight(string& str)
	{
		while (str.size() > 0 && str.back() == ' ')
			str.pop_back();
	}

	void IniFile::TrimBoundary(string& str)
	{
		TrimLeft(str), TrimRight(str);
	}
} // namespace as