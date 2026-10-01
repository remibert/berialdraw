#pragma once

#include <stddef.h>

namespace berialdraw
{
	/** Suffix of a key whose value is stored with a 64th of pixel precision */
	static constexpr char Q6_SUFFIX[] = "_q6";
	/** Key name with the "_q6" suffix appended at compile time, no allocation.
	The suffix tells JsonIterator that the value is stored with a 64th of pixel precision. */
	template<size_t N>
	struct Q6Key
	{
		char buf[N + 3];

		constexpr Q6Key(const char (&name)[N]) : buf{}
		{
			for (size_t i = 0; i + 1 < N; i++)
			{
				buf[i] = name[i];
			}
			buf[N - 1] = '_';
			buf[N]     = 'q';
			buf[N + 1] = '6';
			buf[N + 2] = '\0';
		}

		constexpr operator const char * () const { return buf; }
	};

	/** Build the precise (Q6) variant of a key name */
	template<size_t N>
	constexpr Q6Key<N> q6(const char (&name)[N])
	{
		return Q6Key<N>(name);
	}
}
