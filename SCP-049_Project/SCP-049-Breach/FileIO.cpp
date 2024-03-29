#include "FileIO.h"

FileWriter::FileWriter()
{
}


FileWriter::~FileWriter()
{
}

void FileWriter::SaveLightShapes(std::string path, std::vector<std::shared_ptr<ltbl::LightShape>>& list)
{
	std::ofstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}
	
	file << "start\n";
	sf::ConvexShape ls;
	//stackoverflow
	for (std::vector<std::shared_ptr<ltbl::LightShape>>::iterator it = list.begin(); it != list.end(); ++it)
	{
		ls = it->get()->_shape;
		file << "id:" << it - list.begin() << std::endl;
		file << ls.getPosition().x << ":" << ls.getPosition().y << std::endl;
		file << ls.getPointCount() << std::endl;
		for (int i = 0; i < ls.getPointCount(); i++)
		{
			file << ls.getPoint(i).x << ":" << ls.getPoint(i).y << std::endl;
		}

	}
	file << "end\n";
	file.close();
	/*
	for (std::map<int, std::shared_ptr<ltbl::LightShape>>::iterator it = list.begin(); it != list.end(); ++it)
	{
		ls = it->second.get()->_shape;
		myfile << "id:" << it->first << std::endl;
		myfile << ls.getOrigin().x << ":" << ls.getOrigin().y << std::endl;
		myfile << ls.getPointCount() << std::endl;
		for (int i = 0; i < ls.getPointCount(); i++)
		{
			myfile << ls.getPoint(i).x << ":" << ls.getPoint(i).y << std::endl;
		}

	}*/
}

void FileWriter::LoadLightShapesFromFile(std::string path, std::vector<std::shared_ptr<ltbl::LightShape>>& list, ltbl::LightSystem& ls)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	//unsigned int Index = list.size();
	size_t index = 0,
		pnt_cnt = 0;
	std::string tmp = "";

	do
	{
		std::getline(file, tmp);
		//std::cout << tmp << std::endl;
		//std::cout << "nothing found...\n";
	} while (tmp.compare("start") != 0 && !file.eof());

	if (tmp.compare("start") == 0)
	{
		list.clear();
		//std::cout << "start found!\n";
	}
	else
	{
		std::cout << "something went wrong\n";
		return;
	}

	std::getline(file, tmp, '\n');
	int init = 0;

	do
	{
		//int pos = tmp.find(':');
		size_t pos = tmp.find(':');
		if (tmp.substr(0, pos).compare("id") == 0)
		{
			if(init > 0)
				ls.addShape(list.at(index));

			//index++;
			index = list.size();
			pnt_cnt = 0;
			//std::cout << "*** new light shape ***\nid = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list.push_back(std::make_shared<ltbl::LightShape>());
			std::getline(file, tmp, '\n');
			pos = tmp.find(':');
			//std::cout << "origin: x = " << tmp.substr(0, pos) << "\ty = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list.at(index)->_shape.setPosition( std::stof(tmp.substr(0, pos)), std::stof(tmp.substr(pos + 1, tmp.length() - pos)));
			std::getline(file, tmp, '\n');
			//std::cout << "# of point = " << tmp << std::endl;
			list.at(index)->_shape.setPointCount(std::stoi(tmp));
		}
		else
		{
			//std::cout << "x = " << tmp.substr(0, pos) << "\ty = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list.at(index)->_shape.setPoint(pnt_cnt, sf::Vector2f(std::stof(tmp.substr(0, pos)), std::stof(tmp.substr(pos + 1, tmp.length() - pos))));
			pnt_cnt++;
		}
		std::getline(file, tmp, '\n');
		init = 1;
	} while (tmp.compare("end") != 0 && !file.bad() && !file.eof());

	ls.addShape(list.at(index));
}

void FileWriter::LoadSpawnPoints(const char* path, std::vector<sf::Vector2f>& spawns)
{
	std::vector<sf::Vector2f> sp;

	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	std::string tmp;
	sf::Vector2f vec;

	do {
		std::getline(file, tmp);

		size_t p = tmp.find(':');

		vec.x = atof(tmp.substr(0, p).c_str());
		vec.y =  atof(tmp.substr(p+1).c_str());

		sp.push_back(vec);
		
	} while (!file.eof());

	spawns = sp;
}

std::string FileWriter::LoadText(const char* path)
{
	std::ifstream file(path);
	//file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return "Error";
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	return content;
}
