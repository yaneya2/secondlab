#include "BitSequence.h"
#include "ArraySequence.h"

BitSequence::BitSequence(DynamicArray<uint8_t> *b, size_t bits)
    : bytes(b), bitSize(bits) {
}
uint8_t BitSequence::getByte(size_t byteIdx) const {
    return bytes->Get(byteIdx);
}

void BitSequence::setByte(size_t byteIdx, uint8_t val) {
    bytes->Set(byteIdx, val);
}

bool BitSequence::getBit(size_t bitIdx) const {
    size_t byteIdx = bitIdx / 8;
    uint8_t mask = 1u << (bitIdx % 8);
    return (bytes->Get(byteIdx) & mask) != 0;
}

BitSequence *BitSequence::setBit(size_t bitIdx, bool value) {
    size_t byteIdx = bitIdx / 8;
    uint8_t mask = 1u << (bitIdx % 8);
    uint8_t newByte = bytes->Get(byteIdx);
    if (value) newByte |= mask;
    else newByte &= ~mask;

    DynamicArray<uint8_t> *newBytes = new DynamicArray<uint8_t>(bytes->GetSize());
    for (size_t i = 0; i < bytes->GetSize(); ++i) {
        newBytes->Set(i, i == byteIdx ? newByte : bytes->Get(i));
    }
    return new BitSequence(newBytes, bitSize);
}

size_t BitSequence::numBytes() const {
    return (bitSize + 7) / 8;
}

BitSequence *BitSequence::createCopy() const {
    size_t nb = numBytes();
    DynamicArray<uint8_t> *newBytes = new DynamicArray<uint8_t>(nb);
    for (size_t i = 0; i < nb; ++i) newBytes->Set(i, bytes->Get(i));
    return new BitSequence(newBytes, bitSize);
}

BitSequence *BitSequence::createEmpty() const {
    return new BitSequence(new DynamicArray<uint8_t>(0), 0);
}

BitSequence *BitSequence::instance() {
    return this;
}

BitSequence::BitSequence(const bool *bits, size_t numBits) : bitSize(numBits) {
    size_t nb = numBytes();
    bytes = new DynamicArray<uint8_t>(nb);
    for (size_t i = 0; i < nb; ++i) {
        uint8_t b = 0;
        for (size_t j = 0; j < 8 && 8 * i + j < numBits; ++j) {
            if (bits[8 * i + j]) b |= 1u << j;
        }
        bytes->Set(i, b);
    }
}

BitSequence::BitSequence(const char *bitStr) : bitSize(0) {
    while (bitStr[bitSize] && (bitStr[bitSize] == '0' || bitStr[bitSize] == '1')) ++bitSize;
    if (bitSize == 0) throw std::invalid_argument("Empty bit string");

    size_t nb = numBytes();
    bytes = new DynamicArray<uint8_t>(nb);
    for (size_t i = 0; i < nb; ++i) {
        uint8_t b = 0;
        for (size_t j = 0; j < 8 && 8 * i + j < bitSize; ++j) {
            if (bitStr[8 * i + j] == '1') b |= 1u << j;
        }
        bytes->Set(i, b);
    }
}

bool BitSequence::GetFirst() const {
    if (bitSize == 0) throw std::out_of_range("Empty BitSequence");
    return getBit(0);
}

bool BitSequence::GetLast() const {
    if (bitSize == 0) throw std::out_of_range("Empty BitSequence");
    return getBit(bitSize - 1);
}

bool BitSequence::Get(size_t index) const {
    if (index >= bitSize) throw std::out_of_range("Bit index out of range");
    return getBit(index);
}

size_t BitSequence::GetLength() const {
    return bitSize;
}

BitSequence *BitSequence::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex > endIndex || endIndex > bitSize) throw std::out_of_range("Invalid range");
    BitSequence *sub = createEmpty();
    for (size_t i = startIndex; i < endIndex; ++i) {
        sub->Append(Get(i));
    }
    return sub;
}

IEnumerator<bool> *BitSequence::GetEnumerator() {
    class BitEnumerator : public IEnumerator<bool> {
        const BitSequence *seq;
        size_t pos = 0;

    public:
        BitEnumerator(const BitSequence *s) : seq(s) {
        }

        bool MoveNext() override { return pos < seq->GetLength(); }
        bool Current() const override { return seq->Get(pos); }
        void Reset() override { pos = 0; }

        ~BitEnumerator() override = default;
    };
    return new BitEnumerator(this);
}

BitSequence *BitSequence::bitwiseAndOrXor(const BitSequence *other, std::function<bool(bool, bool)> op) const {
    size_t maxLen = std::max(bitSize, other->bitSize);
    BitSequence *result = createEmpty();
    for (size_t i = 0; i < maxLen; ++i) {
        bool a = i < bitSize ? Get(i) : false;
        bool b = i < other->bitSize ? other->Get(i) : false;
        result->Append(op(a, b));
    }
    return result;
}

BitSequence *BitSequence::And(const BitSequence *other) const {
    return bitwiseAndOrXor(other, [](bool a, bool b) { return a && b; });
}

BitSequence *BitSequence::Or(const BitSequence *other) const {
    return bitwiseAndOrXor(other, [](bool a, bool b) { return a || b; });
}

BitSequence *BitSequence::Xor(const BitSequence *other) const {
    return bitwiseAndOrXor(other, [](bool a, bool b) { return a != b; });
}

BitSequence *BitSequence::Not() const {
    BitSequence *res = createEmpty();
    for (size_t i = 0; i < bitSize; ++i) {
        res->Append(!Get(i));
    }
    return res;
}

template<typename U>
Sequence<U> *BitSequence::Mask(const Sequence<U> *other) const {
    Sequence<U> *res = other->createEmpty();
    size_t minLen = std::min(bitSize, other->GetLength());
    for (size_t i = 0; i < minLen; ++i) {
        if (Get(i)) res = res->Append(other->Get(i));
    }
    return res;
}

BitSequence *BitSequence::appendImpl(const bool &elem) {
    BitSequence *newSeq = createCopy();
    newSeq->bitSize++;
    size_t newNb = newSeq->numBytes();
    if (newNb > newSeq->bytes->GetSize()) newSeq->bytes->Resize(newNb);
    newSeq->setBit(newSeq->bitSize - 1, elem);
    return newSeq;
}

BitSequence *BitSequence::prependImpl(const bool &elem) {
    BitSequence *newSeq = createCopy();
    newSeq->bitSize++;
    size_t newNb = newSeq->numBytes();
    if (newNb > newSeq->bytes->GetSize()) newSeq->bytes->Resize(newNb);
    for (size_t i = newSeq->bitSize - 1; i > 0; --i) {
        newSeq->setBit(i, newSeq->getBit(i - 1));
    }
    newSeq->setBit(0, elem);
    return newSeq;
}

BitSequence *BitSequence::insertAtImpl(const bool &elem, size_t index) {
    if (index > bitSize) throw std::out_of_range("InsertAt index out of range");
    BitSequence *newSeq = createCopy();
    newSeq->bitSize++;
    size_t newNb = newSeq->numBytes();
    if (newNb > newSeq->bytes->GetSize()) newSeq->bytes->Resize(newNb);
    for (size_t i = newSeq->bitSize - 1; i > index; --i) {
        newSeq->setBit(i, newSeq->getBit(i - 1));
    }
    newSeq->setBit(index, elem);
    return newSeq;
}

BitSequence *BitSequence::concatImpl(const Sequence<bool> *other) {
    BitSequence *newSeq = createCopy();
    size_t otherLen = other->GetLength();
    size_t newTotalBits = newSeq->bitSize + otherLen;
    size_t newNb = (newTotalBits + 7) / 8;
    if (newNb > newSeq->bytes->GetSize()) newSeq->bytes->Resize(newNb);
    for (size_t i = 0; i < otherLen; ++i) {
        newSeq->setBit(newSeq->bitSize++, other->Get(i));
    }
    return newSeq;
}

BitSequence::~BitSequence() {
    delete bytes;
}
