/*
 * CORRECT.H
 * Utility for automated checking correctness of tests
 * Copyright (c) Vladimir Yakovlev, 2005-06
 * Version 0.2.1
 *
 * History:
 *  26.04.2005 v.0.1.0. first release
 *  10.08.2005 v.0.1.1. RequireTree(), RequireNotEoln() added
 *  27.09.2005 v.0.1.2. ReadString(), ReadLine(), RequirePrime() added
 *  27.02.2006 v.0.2.0. stricter whitespace control, EOF is not EOLN now
 *  04.04.2006 v.0.2.1. RequireConnected() added
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

typedef std::pair<int, int> IntPair;
typedef std::vector<int> IntVector;
typedef std::vector<IntPair> IntPairVector;
typedef std::queue<int> IntQueue;

#define Assert(expr) _assert((expr), #expr)

void Error(const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(stderr, fmt, argptr);
	fprintf(stderr, " ");
	va_end(argptr);
	exit(1);
}

class InputStream
{
	FILE* m_file;
	int c;
	int line, pos;
	char m_charStr[256][5];
	char m_eofStr[4];

	enum SpecialCharacters
	{
		IS_EOF = EOF,
		IS_BEFORE = -2,
		IS_SPACE = ' ',
		IS_TAB = '\t',
		IS_EOLN = '\n'
	};

	int NextChar()
	{
		if (Eoln())
		{
			line++;
			pos = 1;
		}
		else
			pos++;
		c = getc(m_file);
		return c;
	}

	const char* CharToStr(int c)
	{
		if (c == IS_EOF)
			return m_eofStr;
		else
			return m_charStr[c];
	}

	void Init()
	{
		line = 1;
		pos = 0;
		c = IS_BEFORE;
		NextChar();
		for (int i=0; i<256; i++)
		{
			m_charStr[i][0] = '\'';
			m_charStr[i][1] = i;
			m_charStr[i][2] = '\'';
			m_charStr[i][3] = 0;
		}
		strcpy(m_charStr[IS_TAB], "TAB");
		strcpy(m_charStr[IS_EOLN], "EOLN");
		strcpy(m_eofStr, "EOF");
	}

public:
	InputStream(FILE* file) : m_file(file)
	{
		if (!m_file)
			Error("FILE pointer is invalid");
		Init();
	}

	InputStream(const char* fileName)
	{
		m_file = fopen(fileName, "rt");
		if (!m_file)
			Error("Cannot open file %s", fileName);
		Init();
	}

	~InputStream()
	{
		fclose(m_file);
	}

	bool Blank() const
	{
		return (c == IS_SPACE || c == IS_TAB || c == IS_EOLN);
	}

	bool Digit() const
	{
		return (c >= '0' && c <= '9');
	}

	// !!! EOF is not EOLN
	bool Eoln() const
	{
		return (c == IS_EOLN);
	}

	bool SeekEoln()
	{
		while (!Eoln() && Blank())
			NextChar();
		return Eoln();
	}

	bool Eof() const
	{
		return (c == IS_EOF);
	}

	bool SeekEof()
	{
		while (!Eof() && Blank())
			NextChar();
		return Eof();
	}

	void RequireSpace()
	{
		if (c != IS_SPACE)
			Error("Line %d, pos %d: SPACE expected", line, pos);
		NextChar();
	}

	void RequireEoln(bool skipBlanks = true)
	{
		if (skipBlanks && !SeekEoln() || !skipBlanks && !Eoln())
			Error("Line %d, pos %d: EOLN expected", line, pos);
		NextChar();
	}

	void RequireNotEoln()
	{
		if (SeekEoln())
			Error("Line %d, pos %d: Unexpected EOLN", line, pos);
	}

	void RequireEof(bool skipBlanks = true)
	{
		if (skipBlanks && !SeekEof() || !skipBlanks && !Eof())
			Error("Line %d, pos %d: EOF expected", line, pos);
	}

	int ReadChar()
	{
		int oldc = c;
		NextChar();
		return oldc;
	}

	std::string ReadString()
	{
		std::string res;
		while (Blank())
			NextChar();
		while (!Blank() && !Eof())
		{
			res += (char)c;
			NextChar();
		}
		return res;
	}

	std::string ReadLine(bool readEoln = true)
	{
		std::string res;
		while (!Eoln() && !Eof())
		{
			res += (char)c;
			NextChar();
		}
		if (readEoln && !Eof())
			NextChar();
		return res;
	}

	int ReadInt(bool skipBlanks = true)
	{
		bool minus = false;
		int x = 0;

		if (skipBlanks)
			while (Blank())
				NextChar();
		if (c == '-')
		{
			minus = true;
			NextChar();
		}
		if (!Digit())
			Error("Line %d, pos %d: %s instead of 'int'", line, pos, CharToStr(c));

		while (Digit())
		{
			if (minus)
			{
				if (x < -214748364 || x*10 - -2147483648 < (c-'0'))
					Error("Line %d, pos %d: overflow of 'int'", line, pos);
				x = x*10 - (c-'0');
			}
			else
			{
				if (x > 214748364 || 2147483647 - x*10 < (c-'0'))
					Error("Line %d, pos %d: overflow of 'int'", line, pos);
				x = x*10 + (c-'0');
			}
			NextChar();
		}
		return x;
	}

	unsigned int ReadUInt(bool skipBlanks = true)
	{
		unsigned int x = 0;

		if (skipBlanks)
			while (Blank())
				NextChar();
		if (!Digit())
			Error("Line %d, pos %d: %s instead of 'unsigned int'", line, pos, CharToStr(c));

		while (Digit())
		{
			if (x > 429496729u || 4294967295u - x*10u < (unsigned int)(c-'0'))
				Error("Line %d, pos %d: overflow of 'unsigned int'", line, pos);
			x = x*10u + (unsigned int)(c-'0');
			NextChar();
		}
		return x;
	}

	double ReadDouble(bool skipBlanks = true)
	{
		bool minus = false;
		bool wasDigit = false;
		double x = 0.0;

		if (skipBlanks)
			while (Blank())
				NextChar();

		if (c == '-')
		{
			minus = true;
			NextChar();
		}
		while (Digit())
		{
			x = x*10.0 + (c-'0');
			NextChar();
			wasDigit = true;
		}
		if (c == '.')
		{
			NextChar();
			double y = 0.1;
			while (Digit())
			{
				x += y*(double)(c-'0');
				y *= 0.1;
				NextChar();
				wasDigit = true;
			}
		}
		if (!wasDigit)
			Error("Line %d, pos %d: %s instead of 'double'", line, pos, CharToStr(c));
		if (minus)
			x = -x;
		return x;
	}

	void _assert(bool noerror, const char* expr)
	{
		if (!noerror)
			Error("Condition failed (line %d, pos %d): %s ", line, pos, expr);
	}

};

bool IsConnected(int n, const IntPairVector& edges)
{
	int i;
	std::vector<bool> used(n, false);
	std::vector<IntVector> adj(n);
	IntQueue q;
	for (i=0; i<(int)edges.size(); i++)
	{
		adj[edges[i].first-1].push_back(edges[i].second-1);
		adj[edges[i].second-1].push_back(edges[i].first-1);
	}
	used[0] = true;
	q.push(0);
	while (!q.empty())
	{
		int x = q.front();
		q.pop();
		for (i=0; i<(int)adj[x].size(); i++)
		{
			int y = adj[x][i];
			if (!used[y])
			{
				used[y] = true;
				q.push(y);
			}
		}
	}
	for (i=0; i<n; i++)
		if (!used[i])
			return false;
	return true;
}

void RequireTree(const IntPairVector& tree)
{
	if (!IsConnected(tree.size()+1, tree))
		Error("Graph is not a tree");
}

void RequireConnected(int n, const IntPairVector& edges)
{
	if (!IsConnected(n, edges))
		Error("Graph is not connected");
}

void RequirePrime(int p)
{
	int i;
	for (i=2; i*i<=p; i++)
		if (p % i == 0) break;
	if (p<2 || i*i<=p)
		Error("Number is not a prime: %d", p);
}
