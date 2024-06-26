#include "cl_base.h"

cl_base::cl_base(cl_base* p_head_object, std::string s_object_name)
{
	this->p_head_object = p_head_object;
	this->s_object_name = s_object_name;
	if (p_head_object) p_head_object->subordinate_objects.push_back(this);
}

cl_base* cl_base::get_head_object()
{
	return p_head_object;
}

cl_base* cl_base::get_object_by_name(std::string s_object_name)
{
	cl_base* base = this;
	while (true) if (base->get_head_object()) base = base->get_head_object();
	else break;

	if (base->get_branch_object_by_name(s_object_name)) return base->get_branch_object_by_name(s_object_name);
	return nullptr;
}

cl_base* cl_base::get_sub_object_by_name(std::string s_object_name)
{
	if (!s_object_name.empty())
	{
		for (int i = 0; i < subordinate_objects.size(); i++) if (subordinate_objects.at(i)->get_object_name() == s_object_name) return subordinate_objects.at(i);
		for (int i = 0; i < subordinate_objects.size(); i++) if (subordinate_objects.at(i)->get_sub_object_by_name(s_object_name)) return subordinate_objects.at(i)->get_sub_object_by_name(s_object_name);
	}
	return nullptr;
}

cl_base* cl_base::get_branch_object_by_name(std::string s_object_name)
{
	if (this->s_object_name == s_object_name) return this;

	for (cl_base* subordinate_object : subordinate_objects) if (subordinate_object->get_object_name() == s_object_name) return subordinate_object;
	for (cl_base* subordinate_object : subordinate_objects) if (subordinate_object->get_branch_object_by_name(s_object_name)) return subordinate_object->get_branch_object_by_name(s_object_name);

	return nullptr;
}

cl_base* cl_base::get_object_by_coordinate(std::string s_object_coordinate)
{
	if (!s_object_coordinate.empty())
	{
		cl_base* base = this;
		if (s_object_coordinate.at(0) == '.')
		{
			if (s_object_coordinate.length() == 1) return this;
			return get_branch_object_by_name(s_object_coordinate.substr(1));
		}
		else if (s_object_coordinate.at(0) == '/')
		{
			while (true) if (base->get_head_object()) base = base->get_head_object();
			else break;

			if (s_object_coordinate.length() == 1) return base;
			else if (s_object_coordinate.at(1) == '/') return base->get_sub_object_by_name(s_object_coordinate.substr(2));
			s_object_coordinate = s_object_coordinate.substr(1);
		}
		else if (s_object_coordinate == ".") return this;
		
		std::vector<std::string> names;
		while (s_object_coordinate.length() > 0)
		{
			if (s_object_coordinate.find('/') != std::string::npos)
			{
				names.push_back(s_object_coordinate.substr(0, s_object_coordinate.find('/')));
				s_object_coordinate = s_object_coordinate.substr(s_object_coordinate.find('/') + 1);
			}
			else
			{
				names.push_back(s_object_coordinate);
				break;
			}
		}

		for (std::string name : names)
		{
			base = base->get_sub_object_by_name(name);
			if (!base) break;
		}
		return base;
	}
	return nullptr;
}

std::string cl_base::get_object_name()
{
	return s_object_name;
}

bool cl_base::change_object_name(std::string s_object_name)
{
	if (s_object_name.empty()) return false;
	for (cl_base* subordinate_object : subordinate_objects) if (subordinate_object->get_object_name() == s_object_name) return false;
	
	this->s_object_name = s_object_name;
	return true;
}

void cl_base::change_object_state(int i_object_state)
{
	this->i_object_state = i_object_state;
	if (this->i_object_state == 0) for (cl_base* subordinate_object : subordinate_objects) subordinate_object->change_object_state(i_object_state);

	cl_base* base = this;
	while (true)
	{
		if (base->get_head_object())
		{
			base = base->get_head_object();
			if (i_object_state == 0) base->change_object_state(0);
		}
		else break;
	}
}

bool cl_base::change_head_object(cl_base* p_head_object)
{
	if (!p_head_object || !get_head_object()) return false;

	get_head_object()->delete_sub_object_by_name(get_object_name());
	this->p_head_object = p_head_object;
	this->p_head_object->subordinate_objects.push_back(this);
	return true;
}

void cl_base::delete_sub_object_by_name(std::string s_object_name)
{
	if (s_object_name.empty()) return;	
	for (size_t i = 0; i < subordinate_objects.size(); i++) if (subordinate_objects.at(i)->get_object_name() == s_object_name) subordinate_objects.erase(subordinate_objects.begin() + i);
}

void cl_base::show_object_tree(int depth)
{
	std::cout << std::endl << std::string(depth * 4, ' ') << s_object_name;
	for (cl_base* subordinate_object : subordinate_objects) subordinate_object->show_object_tree(depth + 1);
}

void cl_base::show_traversal_tree(int depth)
{
	std::cout << std::endl << std::string(depth * 4, ' ') << s_object_name << " is " << (i_object_state == 0?"not ":"") << "ready";
	for (cl_base* subordinate_object : subordinate_objects) subordinate_object->show_traversal_tree(depth + 1);
}
