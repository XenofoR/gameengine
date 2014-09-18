#pragma once

#include <stdint.h>
#include <mutex>

static const unsigned int ALIGNMENT = 16;

template <class T>
class MemPool
{
private:
	// Startpointer
	uint32_t* m_start;
	
	uint32_t* m_firstFree;
	std::mutex m_lock;
	unsigned int m_numBlocks;
	unsigned int m_sizeOfBlock;

public:
	
	MemPool(unsigned p_numBlocks, unsigned p_alignment) 
	{
		m_numBlocks = p_numBlocks;
		m_sizeOfBlock = sizeof(T);

		uint32_t* raw = (uint32_t*)malloc(p_numBlocks * m_sizeOfBlock + p_alignment);

		uint32_t mask = p_alignment - 1;
		uint32_t misalignment = ((uint32_t)raw & mask);
		uint32_t adjustment = p_alignment - misalignment;

		m_start = (uint32_t*)((uint32_t)raw + adjustment);
		
		uint32_t* metadata = (uint32_t*)(m_start-4);
		*metadata = adjustment;
		
		m_firstFree = m_start;
		
		uint32_t* currentBlock = m_firstFree;
		for(unsigned int i = 0; i < p_numBlocks-1; i++)
		{
			uint32_t adress = (uint32_t)currentBlock; //get pointer adress as uint
			*currentBlock = (uint32_t)(adress + m_sizeOfBlock); //Save next block as uint in the currentBlock
			currentBlock = reinterpret_cast<uint32_t*>(*currentBlock); //Set currentBlock to next block		
		}
		currentBlock = nullptr;
	}

	~MemPool()
	{
	}
	
	T* getBlock()
	{
		m_lock.lock();
		if(m_firstFree == nullptr)
		{
			//Inget ledigt minne!
		}
		T* local = (T*)m_firstFree;

		m_firstFree = reinterpret_cast<uint32_t*>(*m_firstFree);
		m_lock.unlock();
		return local;
	}

	void freeBlock(T* p_block)
	{
		m_lock.lock();
		uint32_t* block = (uint32_t*)p_block;
		//delete p_block;
		*block = (uint32_t)m_firstFree;
		m_firstFree = block;
		m_lock.unlock();
	}
};
