using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BZTreewidth
{
    // Implements an array of bits, where one 32-bit integer is used to store up to 32 bits
    struct BitArray
    {
        public int[] data;

        public BitArray(int size)
        {
            data = new int[(size + 31) / 32];
        }

        public BitArray(BitArray copy)
        {
            data = (int[])copy.data.Clone();
        }

        public bool this[int i]
        {
            get
            {
                return (data[i / 32] & (1 << (i % 32))) != 0;
            }

            set
            {
                if (value)
                    data[i / 32] |= (1 << (i % 32));
                else
                    data[i / 32] &= ~(1 << (i % 32));
            }
        }

        // Counts the number of bits in the XOR of two arrays
        public int XORCount(BitArray other)
        {
            int result = 0;

            for (int i = 0; i < data.Length; i++)
                result += popcount(data[i] & other.data[i]);

            return result;
        }

        // Snippet: count number of non-zero bits
        int popcount(int v)
        {
            v = v - ((v >> 1) & 0x55555555);
            v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
            return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
        }

        // Implementation inspired by Tuple class
        public override int GetHashCode()
        {
            int hc = 0;
            for (int i = 0; i < data.Length; i++)
                hc = (((data[i] << 5) + data[i]) ^ hc);
            return hc;
        }

        public override bool Equals(object obj)
        {
            BitArray other = (BitArray)obj;

            for (int i = 0; i < data.Length; i++)
                if (data[i] != other.data[i])
                    return false;

            return true;
        }
    }
}
