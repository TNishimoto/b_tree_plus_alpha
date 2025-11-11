#pragma once
#include "./dynamic_bit_sequence.hpp"

namespace stool
{
    namespace bptree
    {
        /// \defgroup WaveletTreeClasses Classes for dynamic wavelet tree
        /// @{
        /// @}

        /**
         * @brief A dynamic data structure supporting rank and select queries on a string \p T[0..n-1] over alphabet \p U[0..σ-1]
         * \ingroup WaveletTreeClasses
         * \ingroup MainClasses
         */
        class DynamicWaveletTree
        {
            using BIT_SEQUENCE = SimpleDynamicBitSequence;
            std::vector<std::vector<BIT_SEQUENCE>> bits_seq;
            std::vector<int64_t> char_rank_vec;
            std::vector<uint8_t> alphabet;
            uint64_t rank_bit_size = 0;

        public:
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Constructors and Destructor
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Default constructor with |T| = 0 and |U| = 0
             */
            DynamicWaveletTree()
            {
                this->alphabet.clear();
            };

            /**
             * @brief Default constructor with |T| = 0 and U = _alphabet
             */
            DynamicWaveletTree(const std::vector<uint8_t> &_alphabet)
            {
                this->set_alphabet(_alphabet);
            };
            /**
             * @brief Destroy the Dynamic Wavelet Tree object
             */
            ~DynamicWaveletTree()
            {
                this->initialize();
            };
            /**
             * @brief Default move constructor.
             */
            DynamicWaveletTree(DynamicWaveletTree &&) noexcept = default;

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operators
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Deleted copy assignment operator.
             */
            DynamicWaveletTree &operator=(const DynamicWaveletTree &) = delete;
            /**
             * @brief Default move assignment operator.
             */
            DynamicWaveletTree &operator=(DynamicWaveletTree &&) noexcept = default;
            /**
             * @brief The alias for at query
             */
            uint8_t operator[](uint64_t i) const
            {
                return this->at(i);
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Lightweight functions for accessing to properties of this class
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Return |T|
             */
            uint64_t size() const
            {
                if (!this->has_empty_alphabet())
                {
                    return this->bits_seq[0][0].size();
                }
                else
                {
                    return 0;
                }
            }

            /**
             * @brief Return alphabet size |U|
             */
            uint64_t get_alphabet_size() const
            {
                return this->alphabet.size();
            }

            /**
             * @brief Check if the alphabet \p U is empty
             */
            bool has_empty_alphabet() const
            {
                return this->rank_bit_size == 0;
            }

            /**
             * @brief Return the smallest character in the alphabet \p U
             */
            uint64_t get_smallest_character_in_alphabet() const
            {
                assert(this->alphabet.size() > 0);
                return this->alphabet[0];
            }
            /**
             * @brief Return the lexicographic order of a given character \p c in the alphabet \p U
             */
            int64_t get_lexicographic_order(uint8_t c) const
            {
                return this->char_rank_vec[c];
            }

            /**
             * @brief Return the height of the wavelet tree
             */
            uint64_t height() const
            {
                return this->bits_seq.size();
            }
            /**
             * @brief Returns the total memory usage in bytes
             * @param only_dynamic_memory If true, only the size of the dynamic memory is returned
             */
            uint64_t size_in_bytes(bool only_dynamic_memory = false) const
            {
                uint64_t total_size_in_bytes = 0;
                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    for (const BIT_SEQUENCE &seq : this->bits_seq[h])
                    {
                        total_size_in_bytes += seq.size_in_bytes(only_dynamic_memory);
                    }
                }
                return total_size_in_bytes;
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Main queries
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Return \p T[i]
             * @note O(log σ log n) time
             */
            uint64_t at(uint64_t i) const
            {
                assert(!this->has_empty_alphabet());
                if (!this->has_empty_alphabet())
                {
                    return this->alphabet[this->get_lexicographic_order_by_position(i)];
                }
                else
                {
                    throw std::range_error("Error: DynamicSequence::at(i)");
                }
            }
            /**
             * @brief Counts the number of occurrences of a character \p c in \p T[0..i-1].
             * @note O(log σ log n) time
             */
            int64_t one_based_rank(uint64_t i, uint64_t c) const
            {
                if (!this->has_empty_alphabet())
                {
                    assert(c < char_rank_vec.size());
                    int64_t c_rank = this->char_rank_vec[c];
                    assert(c_rank <= (int64_t)this->alphabet.size());

                    if (c_rank == -1 || i == 0)
                    {
                        return 0;
                    }
                    else
                    {
                        return this->rank_sub(i - 1, c_rank);
                    }
                }
                else
                {
                    return 0;
                }
            }
            /**
             * @brief Returns the position \p p of the (i+1)-th 1 in \p B if such a position exists, otherwise returns -1
             * @note O(log σ log n) time
             */
            int64_t select(uint64_t i, uint64_t c) const
            {
                if (!this->has_empty_alphabet())
                {
                    assert(c < char_rank_vec.size());
                    int64_t c_rank = this->char_rank_vec[c];
                    assert(c_rank <= (int64_t)this->alphabet.size());

                    if (c_rank == -1)
                    {
                        return -1;
                    }
                    else
                    {
                        return this->select_sub(i + 1, c_rank);
                    }
                }
                else
                {
                    return -1;
                }
            }

            /**
             * @brief Return the lexicographic order of \p T[i] in the alphabet \p U
             * @note O(log σ log n) time
             */
            uint64_t get_lexicographic_order_by_position(uint64_t i) const
            {
                assert(!this->has_empty_alphabet());
                if (!this->has_empty_alphabet())
                {
                    if (i < this->size())
                    {
                        uint64_t next_nth = i;
                        uint64_t j = 0;
                        for (int64_t h = 0; h < (int64_t)this->height(); h++)
                        {
                            bool b = this->bits_seq[h][j].at(next_nth);
                            next_nth = this->bits_seq[h][j].one_based_rank(next_nth, b);
                            j = (j * 2) + (b ? 1 : 0);
                        }
                        return j;
                    }
                    else
                    {
                        throw std::range_error("Error: DynamicSequence::get_nth_char_rank(nth)");
                    }
                }
                else
                {
                    throw std::range_error("Error: DynamicSequence::get_nth_char_rank(nth)");
                }
            }

            /**
             * @brief Return the number of occurrences of the character \p c in \p T
             * @note O(1) time
             */
            uint64_t count_c(uint8_t c) const
            {
                int64_t c_rank = this->get_lexicographic_order(c);
                if (c_rank == -1)
                {
                    return 0;
                }
                else
                {
                    uint64_t idx = (c_rank / 2);
                    uint64_t bit_idx = 0;
                    bool b = stool::LSBByte::get_bit(c_rank, bit_idx);
                    return this->bits_seq[this->bits_seq.size() - 1][idx].count_c(b);
                }
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Conversion functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Return \p T as a string
             */
            std::string to_string() const
            {
                std::string s;
                s.resize(this->size());
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    s[i] = this->at(i);
                }
                return s;
            }

            /**
             * @brief Return \p T as a vector of uint8_t
             */
            std::vector<uint8_t> to_u8_vector() const
            {
                std::vector<uint8_t> s;
                s.resize(this->size());
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    s[i] = this->at(i);
                }
                return s;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Update operations
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Swap operation
             */
            void swap(DynamicWaveletTree &item)
            {
                this->bits_seq.swap(item.bits_seq);

                this->char_rank_vec.swap(item.char_rank_vec);
                this->alphabet.swap(item.alphabet);
                std::swap(this->rank_bit_size, item.rank_bit_size);
            }

            /**
             * @brief Initialize this instance with |T| = 0 and U = _alphabet
             */
            void set_alphabet(const std::vector<uint8_t> &_alphabet)
            {
                this->initialize();

                for (auto c : _alphabet)
                {
                    this->alphabet.push_back(c);
                }
                std::sort(this->alphabet.begin(), this->alphabet.end());

                this->char_rank_vec.resize(256, -1);
                for (uint64_t i = 0; i < alphabet.size(); i++)
                {
                    this->char_rank_vec[alphabet[i]] = i;
                }
                this->rank_bit_size = stool::LSBByte::get_code_length(alphabet.size() - 1);

                for (uint64_t i = 0; i < rank_bit_size; i++)
                {
                    this->bits_seq.push_back(std::vector<BIT_SEQUENCE>());
                    if (i == 0)
                    {
                        this->bits_seq[i].push_back(BIT_SEQUENCE());
                    }
                    else
                    {
                        uint64_t x = 1ULL << i;
                        for (uint64_t j = 0; j < x; j++)
                        {
                            this->bits_seq[i].push_back(BIT_SEQUENCE());
                        }
                    }
                }
            }

            /**
             * @brief Clear the elements in \p T
             */
            void clear()
            {
                for (uint64_t i = 0; i < this->bits_seq.size(); i++)
                {
                    for (uint64_t j = 0; j < this->bits_seq[i].size(); j++)
                    {
                        this->bits_seq[i][j].clear();
                    }
                }
            }
            /**
             * @brief Adds a character \p c to the end of \p T.
             * @note O(log σ log n) time
             */
            void push_back(uint8_t c)
            {
                assert(!this->has_empty_alphabet());
                if (!this->has_empty_alphabet())
                {
                    int64_t c_rank = this->char_rank_vec[c];
                    if (c_rank == -1)
                    {
                        assert(false);
                        throw std::runtime_error("Error: DynamicSequence::push_back()");
                    }
                    else
                    {
                        this->push_back_sub(c_rank);
                    }
                }
                else
                {
                    throw std::runtime_error("Error: DynamicSequence::push_back()");
                }
            }
            /**
             * @brief Add a given sequence \p Q[0..k-1] to the end of \p T[0..n-1] (i.e., \p T = T[0..n-1]Q[0..k-1])
             * @note O(|Q| log σ log n) time
             */
            void push_many(const std::vector<uint8_t> &str)
            {
                for (auto c : str)
                {
                    this->push_back(c);
                }
            }

            /**
             * @brief Insert a character \p c into \p T as \p T[i]
             * @note O(log σ log n) time
             */
            void insert(uint64_t i, uint8_t c)
            {
                assert(!this->has_empty_alphabet());
                if (!this->has_empty_alphabet())
                {
                    int64_t c_rank = this->char_rank_vec[c];
                    if (c_rank == -1)
                    {
                        assert(false);
                        throw std::runtime_error("Error: DynamicSequence::insert(i, c)");
                    }
                    else
                    {
                        this->insert_sub(i, c_rank);
                    }
                }
                else
                {
                    throw std::runtime_error("Error: DynamicSequence::insert(i, c)");
                }
            }
            /**
             * @brief Remove the character at the position \p i from \p T
             * @note O(log σ log n) time
             */
            void remove(uint64_t i)
            {
                if (i < this->size())
                {
                    uint64_t next_nth = i;
                    uint64_t j = 0;
                    for (int64_t h = 0; h < (int64_t)this->height(); h++)
                    {
                        uint64_t current_nth = next_nth;
                        bool b = this->bits_seq[h][j].at(current_nth);
                        next_nth = this->bits_seq[h][j].one_based_rank(current_nth, b);
                        this->bits_seq[h][j].remove(current_nth);
                        j = (j * 2) + (b ? 1 : 0);
                    }
                }
                else
                {
                    throw std::range_error("Error: DynamicSequence::remove(nth)");
                }
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print and verification functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Return the memory usage information of this data structure as a vector of strings
             * @param message_paragraph The paragraph depth of message logs (-1 for no output)
             */
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> r;
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicWaveletTree: " + std::to_string(this->size_in_bytes()) + " bytes =");
                uint64_t counter = 0;
                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    for (const BIT_SEQUENCE &seq : this->bits_seq[h])
                    {
                        if (counter < 5)
                        {
                            std::vector<std::string> log1 = seq.get_memory_usage_info(message_paragraph + 1);
                            for (auto s : log1)
                            {
                                r.push_back(s);
                            }
                        }
                        counter++;
                    }
                }
                if (counter > 5)
                {
                    r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "Omitted details of " + std::to_string(counter - 5) + " dynamic bit sequences.");
                }

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");
                return r;
            }
            /**
             * @brief Print the statistics of this data structure
             * @param message_paragraph The paragraph depth of message logs
             */
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicWaveletTree):" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Alphabet size: " << this->alphabet.size() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Text length: " << this->size() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Estimated memory usage: " << this->size_in_bytes() << " bytes" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }
            /**
             * @brief Print the memory usage information of this data structure
             * @param message_paragraph The paragraph depth of message logs
             */
            void print_memory_usage(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log = this->get_memory_usage_info(message_paragraph);
                for (std::string &s : log)
                {
                    std::cout << s << std::endl;
                }
            }
            /**
             * @brief Print the bit sequence \p T.
             * @param message_paragraph The paragraph depth of message logs
             */
            void print_content(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::string s = this->to_string();
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Content(DynamicWaveletTree):" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "String: " << s << "(" << s.size() << ")" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }
            
            /**
             * @brief Print the statistics of this data structure
             */
            void print_info() const
            {
                std::cout << "================DynamicSequence==================" << std::endl;
                stool::DebugPrinter::print_characters(this->alphabet, "Alphabet");
                stool::DebugPrinter::print_integers(this->char_rank_vec, "Char_rank_vec");
                std::cout << "Rank_bit_size: " << this->rank_bit_size << std::endl;

                for (uint64_t i = 0; i < this->rank_bit_size; i++)
                {
                    std::cout << "level: " << i << " / count: " << this->bits_seq[i].size() << std::endl;
                    for (uint64_t j = 0; j < this->bits_seq[i].size(); j++)
                    {
                        std::cout << this->bits_seq[i][j].to_string() << std::endl;
                    }
                }
                std::cout << "==================================" << std::endl;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Load, save, and builder functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Build a new DynamicWaveletTree from a given sequence \p _text and alphabet \p _alphabet
             */
            static DynamicWaveletTree build(const std::vector<uint8_t> &_text, const std::vector<uint8_t> &_alphabet)
            {
                DynamicWaveletTree dwt(_alphabet);
                dwt.build_bits(_text, 0, 0);
                return dwt;
            }

            /**
             * @brief Returns the DynamicWaveletTree instance loaded from a file stream \p ifs
             */
            static DynamicWaveletTree load_from_file(std::ifstream &ifs)
            {
                DynamicWaveletTree r;
                uint64_t _alphabet_size = 0;
                ifs.read(reinterpret_cast<char *>(&_alphabet_size), sizeof(uint64_t));

                std::vector<uint8_t> _alphabet;
                _alphabet.resize(_alphabet_size);
                ifs.read(reinterpret_cast<char *>(_alphabet.data()), _alphabet_size * sizeof(uint8_t));

                r.set_alphabet(_alphabet);
                for (auto &it : r.bits_seq)
                {
                    for (auto &it2 : it)
                    {
                        auto bits = BIT_SEQUENCE::load_from_file(ifs);
                        it2.swap(bits);
                    }
                }
                return r;
            }
            /**
             * @brief Save the given instance \p item to a file stream \p os
             */
            static void store_to_file(DynamicWaveletTree &item, std::ofstream &os)
            {
                uint64_t alphabet_size = item.alphabet.size();
                os.write(reinterpret_cast<const char *>(&alphabet_size), sizeof(uint64_t));
                os.write(reinterpret_cast<const char *>(item.alphabet.data()), alphabet_size * sizeof(uint8_t));
                for (auto &it : item.bits_seq)
                {
                    for (auto &it2 : it)
                    {
                        BIT_SEQUENCE::store_to_file(it2, os);
                    }
                }
            }
            //@}

        private:
            void build_bits(const std::vector<uint8_t> &_text, uint64_t h, uint64_t i)
            {
                uint64_t bit_idx = this->rank_bit_size - h - 1;
                std::vector<bool> bits;
                bits.resize(_text.size(), false);
                uint64_t counter = 0;
                uint64_t bits1_count = 0;
                uint64_t bits0_count = 0;

                for (uint8_t c : _text)
                {
                    int64_t c_rank = this->char_rank_vec[c];
                    bool b = stool::LSBByte::get_bit(c_rank, bit_idx);
                    bits[counter++] = b;
                    if (b)
                    {
                        bits1_count++;
                    }
                    else
                    {
                        bits0_count++;
                    }
                }
                BIT_SEQUENCE dbs = BIT_SEQUENCE::build(bits);
                this->bits_seq[h][i].swap(dbs);
                if (h + 1 < rank_bit_size)
                {
                    uint64_t next_i = i * 2;
                    {
                        std::vector<uint8_t> left;
                        left.resize(bits0_count);
                        counter = 0;

                        for (uint8_t c : _text)
                        {
                            int64_t c_rank = this->char_rank_vec[c];
                            bool b = stool::LSBByte::get_bit(c_rank, bit_idx);
                            if (!b)
                            {
                                left[counter++] = c;
                            }
                        }
                        this->build_bits(left, h + 1, next_i);
                    }
                    {
                        std::vector<uint8_t> right;
                        right.resize(bits1_count);
                        counter = 0;

                        for (uint8_t c : _text)
                        {
                            int64_t c_rank = this->char_rank_vec[c];
                            bool b = stool::LSBByte::get_bit(c_rank, bit_idx);
                            if (b)
                            {
                                right[counter++] = c;
                            }
                        }
                        this->build_bits(right, h + 1, next_i + 1);
                    }
                }
            }

        private:
            void initialize()
            {
                for (uint64_t i = 0; i < this->bits_seq.size(); i++)
                {
                    for (uint64_t j = 0; j < this->bits_seq[i].size(); j++)
                    {
                        this->bits_seq[i][j].clear();
                        // delete this->bits_seq[i][j];
                    }
                }
                this->bits_seq.clear();
                this->char_rank_vec.clear();
                this->alphabet.clear();
            }

        private:
            void push_back_sub(uint64_t c_rank)
            {
#ifdef DEBUG
                uint64_t _size = this->size();
#endif
                uint64_t pos = 0;
                for (uint64_t i = 0; i < this->rank_bit_size; i++)
                {
                    bool b = stool::LSBByte::get_bit(c_rank, this->rank_bit_size - 1 - i);
                    this->bits_seq[i][pos].push_back(b ? 1 : 0);
                    pos = (pos * 2) + (b ? 1 : 0);
                }

#ifdef DEBUG
                assert(this->size() == _size + 1);
#endif
            }
            int64_t rank_sub(uint64_t i, uint64_t c_rank) const
            {
                bool b1 = stool::LSBByte::get_bit(c_rank, this->rank_bit_size - 1);
                uint64_t _rank = this->bits_seq[0][0].one_based_rank(i + 1, b1);
                if (_rank == 0)
                {
                    return 0;
                }
                else
                {
                    uint64_t nth = _rank - 1;
                    uint64_t next = b1 ? 1 : 0;

                    for (uint64_t i = 1; i < this->rank_bit_size; i++)
                    {
                        bool bx = stool::LSBByte::get_bit(c_rank, this->rank_bit_size - 1 - i);

                        if (nth >= this->bits_seq[i][next].size())
                        {
                            assert(false);
                            throw std::range_error("Error: DynamicSequence::rank_sub()");
                        }

                        uint64_t _rankx = this->bits_seq[i][next].one_based_rank(nth + 1, bx);
                        if (_rankx == 0)
                        {
                            return 0;
                        }
                        else
                        {
                            nth = _rankx - 1;
                            next = (next * 2) + (bx ? 1 : 0);
                        }
                    }
                    return nth + 1;
                }
            }
            int64_t select_sub(uint64_t nth, uint64_t c_rank) const
            {
                int64_t depth = this->bits_seq.size();
                assert(depth > 0);
                int64_t h = 0;
                int64_t j = c_rank / 2;
                bool hbit = stool::LSBByte::get_bit(c_rank, h);
                assert(j < (int64_t)this->bits_seq[depth - h - 1].size());
                uint64_t hbit_rank = this->bits_seq[depth - h - 1][j].count_c(hbit);

                if (nth <= hbit_rank)
                {
                    int64_t hbit_select = this->bits_seq[depth - h - 1][j].select(nth - 1, hbit);

                    assert(hbit_select >= 0);

                    int64_t next_nth = hbit_select + 1;
                    j = j / 2;
                    h++;
                    while (h < (int64_t)this->rank_bit_size)
                    {
                        hbit = stool::LSBByte::get_bit(c_rank, h);
                        assert(next_nth <= this->bits_seq[depth - h - 1][j].count_c(hbit));
                        hbit_select = this->bits_seq[depth - h - 1][j].select(next_nth - 1, hbit);

                        assert(hbit_select >= 0);
                        next_nth = hbit_select + 1;
                        j = j / 2;
                        h++;
                    }
                    assert(next_nth > 0);
                    return next_nth - 1;
                }
                else
                {
                    return -1;
                }
            }
            void insert_sub(uint64_t nth, uint8_t c_rank)
            {
                if (nth <= this->size())
                {

                    uint64_t next_nth = nth;
                    uint64_t j = 0;
                    for (int64_t h = 0; h < (int64_t)this->height(); h++)
                    {
                        bool hbit = stool::LSBByte::get_bit(c_rank, this->rank_bit_size - h - 1);
                        assert(next_nth <= this->bits_seq[h][j].size());
                        this->bits_seq[h][j].insert(next_nth, hbit);
                        next_nth = this->bits_seq[h][j].one_based_rank(next_nth, hbit);
                        j = (j * 2) + (hbit ? 1 : 0);
                    }
                }
                else
                {
                    throw std::range_error("Error: DynamicSequence::insert(nth, c_rank)");
                }
            }
            void build_from_text(const std::vector<uint8_t> &_text, const std::vector<uint8_t> &_alphabet)
            {
                DynamicWaveletTree dwt = DynamicWaveletTree::build(_text, _alphabet);
                this->swap(dwt);
            }
        };

        // template <typename T>
        // using VLCDequeSeq = DynamicSequence<VLCDeque, T>;
    }

}