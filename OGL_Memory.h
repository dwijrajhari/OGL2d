class MemManager
{
public:
	void* MemAllocate(long);
	void MemFree();
private:
	long m_MemCount;
};