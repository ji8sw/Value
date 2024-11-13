#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
constexpr char SPLITTER = ':';

namespace Value
{
	struct Value
	{
		std::string Key = "";
		std::string value = "";
		bool Valid = false;

		std::string ToString(char Delimiter = SPLITTER) const 
		{
			return Key + Delimiter + value;
		}

		Value(std::string Raw = "", bool RemoveWhitespace = true)
		{
			if (!Raw.empty())
			{
				size_t Position = Raw.find(SPLITTER);

				if (RemoveWhitespace) Raw.erase(std::remove_if(Raw.begin(), Raw.end(), ::isspace), Raw.end());

				if (Position != std::string::npos)
				{
					this->Key = Raw.substr(0, Position);
					this->value = Raw.substr(Position + 1);
					this->Valid = true;
				}
			}
		}
	};

	Value FromLine(std::string& Line, bool RemoveWhitespace = true)
	{
		size_t Position = Line.find(SPLITTER);
		Value NewValue;

		if (RemoveWhitespace) Line.erase(std::remove_if(Line.begin(), Line.end(), ::isspace), Line.end());

		if (Position != std::string::npos)
		{
			NewValue.Key = Line.substr(0, Position);
			NewValue.value = Line.substr(Position + 1);
			NewValue.Valid = true;
		}

		return NewValue;
	}

	class Set
	{
	public:
		std::vector<Value> List;

		// Retrieves new Values from the file and appends to the list of Values
		bool AppendFrom(const std::string& FileName, bool RemoveWhitespace = true, bool AllowInvalid = false)
		{
			std::ifstream File(FileName);

			if (!File.is_open()) 
				return false;

			std::string Line;
			while (std::getline(File, Line)) 
			{
				Value NewValue = FromLine(Line, RemoveWhitespace);
				if (NewValue.Valid && !AllowInvalid) List.push_back(NewValue);
			}

			return true;
		}

		std::string ToString(bool AllowInvalid = false)
		{
			std::string Result = "";

			for (Value& Val : List)
			{
				if (!Val.Valid && !AllowInvalid) continue;
				Result += Val.ToString() + '\n';
			}

			return Result;
		}

		bool SaveTo(std::string FileName, bool AllowInvalid = false)
		{
			std::ofstream File(FileName);

			if (!File.is_open())
				return false;

			File << ToString(AllowInvalid);

			return true;
		}

		void Clear() { List.clear(); }
		void Add(Value ToAdd) { List.push_back(ToAdd); }

		Value Get(const std::string& Key) const 
		{
			for (const Value& Val : List) 
			{
				if (Val.Key == Key) return Val;
			}
			return Value{};
		}

		bool Contains(const std::string& Key) const 
		{
			return std::any_of(List.begin(), List.end(), [&Key](const Value& Val) 
			{
				return Val.Key == Key;
			});
		}

		Set(const std::string& FileName = "", bool RemoveWhitespace = true, bool AllowInvalid = false)
		{
			if (!FileName.empty())
				AppendFrom(FileName, RemoveWhitespace, AllowInvalid);
		}
	};
}