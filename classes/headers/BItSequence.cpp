// #include "BItSequence.h"
//
// #include <cstdint>
//
// void BitSequence::setBit(size_t index, Bit value) {
//     size_t byteIdx = index / 8;
//     size_t bitIdx = index % 8;
//     uint8_t byte = data.Get(byteIdx);
//     if (value == Bit::One)
//         byte |= (1 << bitIdx);
//     else
//         byte &= ~(1 << bitIdx);
// }
//
// Bit BitSequence::getBit(size_t index) const {
//     size_t byteIdx = index / 8;
//     size_t bitIdx = index % 8;
//     return (data.Get(byteIdx) & (1 << bitIdx)) ? Bit::One : Bit::Zero;
// }
//
// void BitSequence::ensureCapacity(size_t newBitCount) {
//     size_t neededBytes = (newBitCount + 7) / 8;
//     if (neededBytes > data.GetSize()) {
//         data.Resize(neededBytes);
//         // Новые байты автоматически инициализируются нулями (DynamicArray::Resize вызывает T())
//     }
// }
//
// // ---------- constructors ----------
// BitSequence::BitSequence() : bitCount(0), data(0) {}
//
// BitSequence::BitSequence(const Bit* items, size_t count) : bitCount(count), data(items) {
//     size_t byteCount = (count + 7) / 8;
//     data.Resize(byteCount);
//     for (size_t i = 0; i < byteCount; ++i)
//         data.Set(i, 0);
//     for (size_t i = 0; i < count; ++i)
//         setBit(i, items[i]);
// }
//
// BitSequence::BitSequence(size_t count, Bit defaultValue) : bitCount(count) {
//     size_t byteCount = (count + 7) / 8;
//     data.Resize(byteCount);
//     if (defaultValue == Bit::One) {
//         for (size_t i = 0; i < byteCount; ++i)
//             data.Set(i, 0xFF);
//         // Корректируем последний байт, если count не кратен 8
//         size_t lastByteBits = count % 8;
//         if (lastByteBits != 0) {
//             uint8_t mask = (1 << lastByteBits) - 1;
//             data.Set(byteCount - 1, data.Get(byteCount - 1) & mask);
//         }
//     } else {
//         for (size_t i = 0; i < byteCount; ++i)
//             data.Set(i, 0);
//     }
// }
//
// BitSequence::BitSequence(const std::string& bitString) : bitCount(bitString.size()) {
//     size_t byteCount = (bitString.size() + 7) / 8;
//     data.Resize(byteCount);
//     for (size_t i = 0; i < byteCount; ++i)
//         data.Set(i, 0);
//     for (size_t i = 0; i < bitString.size(); ++i) {
//         if (bitString[i] == '1')
//             setBit(i, Bit::One);
//         else if (bitString[i] == '0')
//             setBit(i, Bit::Zero);
//         else
//             throw std::invalid_argument("Invalid character in bit string");
//     }
// }
//
// BitSequence::BitSequence(const BitSequence& other) : bitCount(other.bitCount) {
//     size_t byteCount = other.data.GetSize();
//     data.Resize(byteCount);
//     for (size_t i = 0; i < byteCount; ++i)
//         data.Set(i, other.data.Get(i));
// }
//
// // ---------- read operations ----------
// Bit BitSequence::GetFirst() const {
//     if (bitCount == 0)
//         throw std::out_of_range("Sequence is empty");
//     return getBit(0);
// }
//
// Bit BitSequence::GetLast() const {
//     if (bitCount == 0)
//         throw std::out_of_range("Sequence is empty");
//     return getBit(bitCount - 1);
// }
//
// Bit BitSequence::Get(size_t index) const {
//     if (index >= bitCount)
//         throw std::out_of_range("Index out of range");
//     return getBit(index);
// }
//
// Sequence<Bit>* BitSequence::GetSubsequence(size_t startIndex, size_t endIndex) const {
//     if (startIndex > endIndex || endIndex >= bitCount)
//         throw std::out_of_range("Invalid subsequence bounds");
//     size_t newLen = endIndex - startIndex + 1;
//     Bit* items = new Bit[newLen];
//     for (size_t i = 0; i < newLen; ++i)
//         items[i] = getBit(startIndex + i);
//     BitSequence* result = new BitSequence(items, newLen);
//     delete[] items;
//     return result;
// }
//
// size_t BitSequence::GetLength() const {
//     return bitCount;
// }
//
// // ---------- modification operations (mutable) ----------
// Sequence<Bit>* BitSequence::Append(const Bit& elem) {
//     ensureCapacity(bitCount + 1);
//     setBit(bitCount, elem);
//     ++bitCount;
//     return this;
// }
//
// Sequence<Bit>* BitSequence::Prepend(const Bit& elem) {
//     ensureCapacity(bitCount + 1);
//     for (size_t i = bitCount; i > 0; --i)
//         setBit(i, getBit(i - 1));
//     setBit(0, elem);
//     ++bitCount;
//     return this;
// }
//
// Sequence<Bit>* BitSequence::InsertAt(const Bit& elem, size_t index) {
//     if (index > bitCount)
//         throw std::out_of_range("Index out of range");
//     ensureCapacity(bitCount + 1);
//     for (size_t i = bitCount; i > index; --i)
//         setBit(i, getBit(i - 1));
//     setBit(index, elem);
//     ++bitCount;
//     return this;
// }
//
// Sequence<Bit>* BitSequence::Concat(Sequence<Bit>* other) {
//     size_t otherLen = other->GetLength();
//     ensureCapacity(bitCount + otherLen);
//     for (size_t i = 0; i < otherLen; ++i)
//         Append(other->Get(i));
//     return this;
// }
//
// // ---------- bitwise operations (return new objects) ----------
// BitSequence BitSequence::And(const BitSequence& other) const {
//     if (bitCount != other.bitCount)
//         throw std::invalid_argument("Bit sequences must have same length for AND");
//     BitSequence result(bitCount, Bit::Zero);
//     for (size_t i = 0; i < bitCount; ++i)
//         if (getBit(i) == Bit::One && other.getBit(i) == Bit::One)
//             result.setBit(i, Bit::One);
//     return result;
// }
//
// BitSequence BitSequence::Or(const BitSequence& other) const {
//     if (bitCount != other.bitCount)
//         throw std::invalid_argument("Bit sequences must have same length for OR");
//     BitSequence result(bitCount, Bit::Zero);
//     for (size_t i = 0; i < bitCount; ++i)
//         if (getBit(i) == Bit::One || other.getBit(i) == Bit::One)
//             result.setBit(i, Bit::One);
//     return result;
// }
//
// BitSequence BitSequence::Xor(const BitSequence& other) const {
//     if (bitCount != other.bitCount)
//         throw std::invalid_argument("Bit sequences must have same length for XOR");
//     BitSequence result(bitCount, Bit::Zero);
//     for (size_t i = 0; i < bitCount; ++i)
//         if (getBit(i) != other.getBit(i))
//             result.setBit(i, Bit::One);
//     return result;
// }
//
// BitSequence BitSequence::Not() const {
//     BitSequence result(bitCount, Bit::Zero);
//     for (size_t i = 0; i < bitCount; ++i)
//         if (getBit(i) == Bit::Zero)
//             result.setBit(i, Bit::One);
//     return result;
// }
//
// // ---------- additional ----------
// std::string BitSequence::ToString() const {
//     std::string s;
//     s.reserve(bitCount);
//     for (size_t i = 0; i < bitCount; ++i)
//         s.push_back(bitToChar(getBit(i)));
//     return s;
// }
//
// IEnumerator<Bit>* BitSequence::GetEnumerator() const {
//     return createEnumerator();
// }
//
// // ---------- protected methods (Sequence<Bit> interface) ----------
// Sequence<Bit>* BitSequence::instance() {
//     return this;
// }
//
// Sequence<Bit>* BitSequence::appendImpl(const Bit& elem) {
//     return Append(elem);
// }
//
// Sequence<Bit>* BitSequence::prependImpl(const Bit& elem) {
//     return Prepend(elem);
// }
//
// Sequence<Bit>* BitSequence::insertAtImpl(const Bit& elem, size_t index) {
//     return InsertAt(elem, index);
// }
//
// Sequence<Bit>* BitSequence::concatImpl(Sequence<Bit>* other) {
//     return Concat(other);
// }
//
// Sequence<Bit>* BitSequence::createEmpty() const {
//     return new BitSequence();
// }
//
// IEnumerator<Bit>* BitSequence::createEnumerator() const {
//     class BitEnumerator : public IEnumerator<Bit> {
//         const BitSequence* seq;
//         size_t pos;
//         size_t len;
//     public:
//         BitEnumerator(const BitSequence* s) : seq(s), pos(0), len(s->GetLength()) {}
//         bool MoveNext() override {
//             if (pos < len) {
//                 ++pos;
//                 return pos <= len;
//             }
//             return false;
//         }
//         Bit Current() const override {
//             if (pos == 0 || pos > len)
//                 throw std::out_of_range("Enumerator out of range");
//             return seq->Get(pos - 1);
//         }
//         void Reset() override { pos = 0; }
//     };
//     return new BitEnumerator(this);
// }