#pragma once
#include <string.h>
#include <new>
#include <memory>
#include <utility>

template<typename T, class Alloc = std::allocator<T>, size_t szBlockMaxSize = 4, size_t szMinMapLength = 8>
class my_deque
{
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using map_pointer = value_type**;
	using allocator_type = Alloc;
protected:
	using block_alloc = typename std::allocator_traits<allocator_type>::template rebind_alloc<value_type>;
	using map_alloc = typename std::allocator_traits<allocator_type>::template rebind_alloc<pointer>;

	static constexpr const size_type szValueSize = sizeof(value_type);
	static constexpr const size_type szBlockLength = szValueSize < szBlockMaxSize ? szBlockMaxSize / szValueSize
		: 1;
	static constexpr const size_type szBlockSize = szBlockLength * szValueSize;
	static constexpr const size_type szMaxMapLength = ((size_type)-1) / szBlockSize;
	static constexpr const size_type szMaxElemCount = szMaxMapLength * szBlockLength;
private:
	map_pointer valBlockMap;
	size_type szMapLength;
	size_type szBeginOff;
	size_type szElemCount;
private:
	map_pointer CopyMap(map_pointer mpDst, map_pointer mpSrc, size_type szCount)
	{
		return (map_pointer)memcpy(mpDst, mpSrc, szCount * sizeof(pointer)) + szCount;
	}
	map_pointer ZeroMap(map_pointer mpDst, size_type szCount)
	{
		return (map_pointer)memset(mpDst, 0, szCount * sizeof(pointer)) + szCount;
	}


	/*
	blkLen=2,MapLen=8,ElmCnt=14
	End:      |
	Beg:      V   V
	Blk:B C D     0 1 2 3 4 5 6 7 8 9 A
	Map:0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |

	blkLen=2,MapLen=16,ElmCnt=14
	End:                                          |
	Beg:          V                               V
	Blk:          0 1 2 3 4 5 6 7 8 9 A B C D E F
	Map:0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F |
	*/
	void GrowMap(size_type szGrowLength)
	{
		size_type szNewLength = szMapLength < szMinMapLength ? szMinMapLength : szMapLength;

		while (szNewLength - szMapLength < szGrowLength)
		{
			if (szNewLength > szMaxMapLength - szNewLength)
			{
				throw("my_deque too long!");
			}

			szNewLength *= 2;
		}

		szGrowLength = szNewLength - szMapLength;
		map_alloc MapAlloc;

		size_type szMapOff = szBeginOff / szBlockLength;
		map_pointer valNewMap = MapAlloc.allocate(szNewLength);
		map_pointer valCpyMap = valNewMap;

		if (valBlockMap != nullptr)
		{
			map_alloc MapAlloc;
			valCpyMap = ZeroMap(valCpyMap, szMapOff);//最前部分置0
			valCpyMap = CopyMap(valCpyMap, valBlockMap + szMapOff, szMapLength - szMapOff);//开始位置拷贝到新开始位置
			valCpyMap = CopyMap(valCpyMap, valBlockMap, szMapOff);//后半部分拷贝到后面
			valCpyMap = ZeroMap(valCpyMap, szGrowLength - szMapOff);//最后部分置0
			MapAlloc.deallocate(valBlockMap, szMapLength);
		}
		else
		{
			valCpyMap = ZeroMap(valCpyMap, szNewLength);//全部置零
		}

		valBlockMap = valNewMap;
		szMapLength = szNewLength;
	}

	void Tidy(void)
	{
		while (!empty())//此处清理并设置szBeginOff和szElemCount
		{
			pop_back();
		}

		block_alloc BlockAlloc{};
		for (size_type szBlockIdx = 0; szBlockIdx < szMapLength; ++szBlockIdx)
		{
			BlockAlloc.deallocate(valBlockMap[szBlockIdx], szBlockLength);
		}

		if (valBlockMap != nullptr)
		{
			map_alloc MapAlloc;
			MapAlloc.deallocate(valBlockMap, szMapLength);
			valBlockMap = nullptr;
			szMapLength = 0;
		}
	}

	size_type GetBlock(size_type szOff)
	{
		return (szOff / szBlockLength) & (szMapLength - 1);//szMapLength必须为2的倍数
	}

	value_type* GetElemPtr(size_type szOff)
	{
		size_type szBlock = GetBlock(szOff);
		size_type szElem = szOff % szBlockLength;
		return &valBlockMap[szBlock][szElem];
	}


	void RefCopy(const my_deque& Copy)
	{
		throw "error call";
	}

	void RefMove(my_deque&& Move)
	{
		valBlockMap = Move.valBlockMap;
		szMapLength = Move.szMapLength;
		szBeginOff = Move.szBeginOff;
		szElemCount = Move.szElemCount;

		Move.valBlockMap = nullptr;
		Move.szMapLength = 0;
		Move.szBeginOff = 0;
		Move.szElemCount = 0;
	}


public:
	my_deque(void) :valBlockMap(nullptr), szMapLength(0), szBeginOff(0), szElemCount(0)
	{}
	~my_deque(void)
	{
		Tidy();
	}

	my_deque(const my_deque& Copy)
	{
		RefCopy(Copy);
	}

	my_deque(my_deque&& Move)
	{
		RefMove(std::move(Move));
	}

	my_deque& operator=(const my_deque& Copy)
	{


		return *this;
	}
	my_deque& operator=(my_deque&& Move)
	{


		return *this;
	}



	allocator_type get_allocator(void) const
	{
		return allocator_type{};
	}

	size_type max_size(void) const
	{
		return szMaxElemCount;
	}

	bool empty(void) const
	{
		return szElemCount == 0;
	}

	size_type size(void) const
	{
		return szElemCount;
	}

	void clear(void)
	{
		Tidy();
	}

	template <typename... Valty>
	void emplace_front(Valty&&... Val)
	{
		if (szBeginOff % szBlockLength == 0 && szElemCount / szBlockLength + 1 >= szMapLength)
		{
			GrowMap(1);
		}

		szBeginOff &= szMapLength * szBlockLength - 1;
		size_type szNewOff = (szBeginOff != 0 ? szBeginOff : szMapLength * szBlockLength) - 1;
		size_type szBlock = GetBlock(szNewOff);
		size_type szElem = szNewOff % szBlockLength;
		if (valBlockMap[szBlock] == nullptr)
		{
			block_alloc BlockAlloc;
			valBlockMap[szBlock] = BlockAlloc.allocate(szBlockLength);
		}


		std::construct_at(&valBlockMap[szBlock][szElem], std::forward<Valty>(Val)...);//new(&valBlockMap[szBlock][szElem]) value_type(std::forward<Valty>(Val)...);
		szBeginOff = szNewOff;
		++szElemCount;
	}

	void push_front(const value_type& Val)
	{
		emplace_front(Val);
	}

	template <typename... Valty>
	void emplace_back(Valty&&... Val)
	{
		if ((szBeginOff + szElemCount) % szBlockLength == 0 && szElemCount / szBlockLength + 1 >= szMapLength)
		{
			GrowMap(1);
		}

		szBeginOff &= szMapLength * szBlockLength - 1;
		size_type szNewOff = szBeginOff + szElemCount;
		size_type szBlock = GetBlock(szNewOff);
		size_type szElem = szNewOff % szBlockLength;
		if (valBlockMap[szBlock] == nullptr)
		{
			block_alloc BlockAlloc;
			valBlockMap[szBlock] = BlockAlloc.allocate(szBlockLength);
		}

		std::construct_at(&valBlockMap[szBlock][szElem], std::forward<Valty>(Val)...);//new(&valBlockMap[szBlock][szElem]) value_type(std::forward<Valty>(Val)...);
		++szElemCount;
	}

	void push_back(const value_type& Val)
	{
		emplace_back(Val);
	}

	void pop_front(void)
	{
		std::destroy_at(GetElemPtr(szBeginOff));//GetElemPtr(szBeginOff)->~value_type();
		--szElemCount;
		if (szElemCount == 0)
		{
			szBeginOff = 0;
		}
		else
		{
			++szBeginOff;
		}
	}

	void pop_back(void)
	{
		std::destroy_at(GetElemPtr(szBeginOff + szElemCount - 1));//GetElemPtr(szBeginOff + szElemCount - 1)->~value_type();
		--szElemCount;
		if (szElemCount == 0)
		{
			szBeginOff = 0;
		}
	}

	void resize(size_type szNewCount, const value_type& Val)
	{
		while (szElemCount < szNewCount)
		{
			push_back(Val);
		}

		while (szElemCount > szNewCount)
		{
			pop_back();
		}
	}


	value_type& front(void)
	{
		return *GetElemPtr(szBeginOff);
	}

	value_type& back(void)
	{
		return *GetElemPtr(szBeginOff + szElemCount - 1);
	}

	value_type& operator[](size_type szOff)
	{
		return *GetElemPtr(szOff + szBeginOff);
	}

	value_type& at(size_type szOff)
	{
		if (szOff >= szElemCount)
		{
			throw("out of range!");
		}
		return *GetElemPtr(szOff + szBeginOff);
	}
};