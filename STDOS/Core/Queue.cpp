#include "Queue.h"

void ExitCritical()
{
    ;
}

void EnterCritical()
{
    ;
}

Queue::Queue(): _s((void*)0, 0)
{
    this->Clear();
}


void Queue::SetCapacity(int len)
{
    this->_s.SetLength(len);
}

void Queue::Clear()
{
    int v2 = this->_s.Capacity();
    this->_s.SetLength(v2);
    this->_head = 0;
    this->_tail = 0;
    this->_size = 0;
}

void Queue::Enqueue(byte dat)
{
    int capacity = this->Capacity();
    if (capacity == 0)
    {
        this->_s.SetLength(64);
    }
    int ret = this->_size;

    if (ret < capacity)
    {
        int headold = this->_head++;
        this->_s[headold] = dat;
        if (this->_head >= capacity)
            this->_head -= capacity;
        EnterCritical();
        this->_size++;
        ExitCritical();
    }
}

byte Queue::Dequeue()
{
    byte ret;

    if (this->_size)
    {
        EnterCritical();
        --this->_size;
        ExitCritical();
        int capacity = this->Capacity();
        int tailold = this->_tail++;
        ret = this->_s[tailold];
        if (this->_tail >= capacity)
        {
            this->_tail -= capacity;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

// 批量写入
int Queue::Write(const Buffer &bs)
{
    int ret = 0;
    int capacity = this->Capacity();
    if (capacity==0)
        this->_s.SetLength(64);
    if (this->_size < capacity)
    {		
        int bslen = bs.Length();
        if (this->_size + bslen > capacity)
        {
            bslen = capacity - this->_size;
        }
		for(int i=0;i<bslen;i++)
		{
			this->Enqueue(bs[i]);
		}
		ret=this->Length();
    }
    else
    {
        ret = 0;
    }

    return ret;
}

// 批量读取
int Queue::Read(Buffer &bs)
{
    int ret = 0;
    if (this->_size != 0)
    {
        int buflen = bs.Length();
        if (buflen != 0)
        {
            if (this->_size < buflen)
            {
                buflen = this->_size;
            }
            for (int i = 0; i < buflen; i++)
            {
                bs[i] = this->Dequeue();
            }
			bs.SetLength(buflen);
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}
FIFO::FIFO()
{
    this->pBuf = 0;
    this->BufSize = 0;
    this->Read = 0;
    this->Write = 0;
}

void FIFO::SetBuf(void *buf, int len)
{
    if (len >= 0)
    {
        this->pBuf = (byte*)buf;
        this->BufSize = len;
    }
    this->Clear();
}

void FIFO::Clear()
{
    this->Write = 0;
    this->Read = 0;
    this->Count = 0;
}

int FIFO::Push(byte da)
{
    if (this->pBuf == 0)
    {
        return  - 1;
    }
    this->pBuf[this->Write] = da;
    if (++this->Write >= this->BufSize)
    {
        this->Write = 0;
    }
    if (this->Count < this->BufSize)
    {
        this->Count++;
        return 0;
    }
    else
    {
        return  - 1;
    }
}

int FIFO::Pop(byte *da)
{
    *da = this->pBuf[this->Read];
    if (++this->Read >= this->BufSize)
    {
        this->Read = 0;
    }
    if (this->Count)
    {
        this->Count--;
        return 0;
    }
    else
    {
        return  - 1;
    }
}

bool FIFO::BufIsEmpty()
{
    return this->Count == 0;
}

bool FIFO::BufIsFull()
{
    return this->Count >= this->BufSize;
}

