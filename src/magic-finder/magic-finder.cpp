/// @file   magic-finder.cpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains the entry point of the magic-finder tool for m8.

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <random>
#include <memory>

#include <boost/io/ios_state.hpp>

#include "../common/Bb.hpp"
#include "../common/Sq.hpp"

namespace m8 {

    /// Generates the occupation mask for a rook or a bishop
    /// 
    /// @param sq      Square where the piece is placed.
    /// @param is_rook Indicate if we generate the mask of a rook or a bishop.
    /// @returns A bitboard with bit sets on all the squares for wich the occupation is   
    /// relevant to generate the moves of the piece on the squared received in parameter.
    Bb GenerateMask(Sq sq, bool is_rook)
    {
        Bb bb = BB_C(0);

        if (is_rook)
        {
            // Go north
            if (GetRow(sq) < kRow7)
                for (Sq target = sq + 8; IsSqOnBoard(target) && GetRow(target) < kRow8; target += 8)
                    SetBit(bb, target);

            // Go south
            if (GetRow(sq) > kRow2)
                for (Sq target = sq - 8; IsSqOnBoard(target) && GetRow(target) > kRow1; target -= 8)
                    SetBit(bb, target);

            // Go west
            if (GetColmn(sq) > kColmnB)
                for (Sq target = sq - 1; IsSqOnBoard(target) && GetColmn(target) > kColmnA; target -= 1)
                    SetBit(bb, target);

            // Go east
            if (GetColmn(sq) < kColmnG)
                for (Sq target = sq + 1; IsSqOnBoard(target) && GetColmn(target) < kColmnH; target += 1)
                    SetBit(bb, target);
        }
        else
        {
            // Go northeast
            if (GetColmn(sq) < kColmnG && GetRow(sq) < kRow7)
                for (Sq target = sq + 9; IsSqOnBoard(target) && GetColmn(target) < kColmnH && GetRow(target) < kRow8; target += 9)
                    SetBit(bb, target);

            // Go southeast
            if (GetColmn(sq) < kColmnG && GetRow(sq) > kRow2)
                for (Sq target = sq - 7; IsSqOnBoard(target) && GetColmn(target) < kColmnH && GetRow(target) > kRow1; target -= 7)
                    SetBit(bb, target);

            // Go southwest
            if (GetColmn(sq) > kColmnB && GetRow(sq) > kRow2)
                for (Sq target = sq - 9; IsSqOnBoard(target) && GetColmn(target) > kColmnA && GetRow(target) > kRow1; target -= 9)
                    SetBit(bb, target);

            // Go northwest
            if (GetColmn(sq) > kColmnB && GetRow(sq) < kRow7)
                for (Sq target = sq + 7; IsSqOnBoard(target) && GetColmn(target) > kColmnA && GetRow(target) < kRow8; target += 7)
                    SetBit(bb, target);
        }
        return bb;
    }

    /// Distributes the bits of bits into the position provided by mask.
    ///
    /// @param bits Integer containing the bits to distribute into mask.
    /// @param mask Mask representing the positions into wich the bits must be 
    ///             distributed.
    /// @return A bitboard.
    Bb DistributeBits(std::uint32_t bits, Bb mask)
    {
        Bb result = BB_C(0);
        std::uint64_t bit_count = GetPopct(mask);

        for (std::uint32_t x = 0; x < bit_count; ++x)
        {
            std::uint32_t pos = RemoveLsb(mask);
            if ((bits & (1 << x)) != 0)
            {
                SetBit(result, pos);
            }
        }

        return result;
    }

    /// For a given mask, generate all variations of values using only the bits in the 
    /// mask.
    ///
    /// @param mask Mask representing the bits that should be used to create the 
    ///             variations.
    /// @return A vector of bitboard containing all occupancies variations.
    std::vector<Bb> GenerateOccupancyVariation(Bb mask)
    {
        std::uint64_t bit_count = GetPopct(mask);
        std::uint32_t num_variation = 1 << bit_count;
        std::vector<Bb> occupancies;

        for (std::uint32_t x = 0; x < num_variation; ++x)
        {
            occupancies.push_back(DistributeBits(x, mask));
        }

        return occupancies;
    }

    /// Generates the attack set of a piece on a given square and occupancy. The attack
    /// sets represents the positions that are occupied and directly attacked by the piece
    /// on a given square. Postiions behind the first attacked postion are not part of the
    /// attack set.
    ///
    /// @param occupancy Bitboard indicating wich positions are occupied.
    /// @param sq        Position of the attacking piece.
    /// @return A bitboard representing all occupied positions dirrectly attacked by the
    ///         attacking piece.
    Bb GenerateAttackSet(Bb occupancy, Sq sq)
    {
        assert(IsSqOnBoard(sq));

        Bb attack_set = BB_C(0);
        Bb bb;

        const std::uint32_t kNumDir = 2;
        std::array <Row, kNumDir> max_rows = { kRow7 };
        
        // Go north
        bb = occupancy & kBbColmn[GetColmn(sq)] & ~(GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetLsb(bb));

        // Go south
        bb = occupancy & kBbColmn[GetColmn(sq)] & (GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetMsb(bb));

        // Go west
        bb = occupancy & kBbRow[GetRow(sq)] & (GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetMsb(bb));

        // Go east
        bb = occupancy & kBbRow[GetRow(sq)] & ~(GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetLsb(bb));

        // Go northeast
        bb = occupancy & kBbDiag[GetDiag(sq)] & ~(GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetLsb(bb));

        // Go southeast
        bb = occupancy & kBbAntiDiag[GetAntiDiag(sq)] & (GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetMsb(bb));

        // Go southwest
        bb = occupancy & kBbDiag[GetDiag(sq)] & (GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetMsb(bb));

        // Go northwest
        bb = occupancy & kBbAntiDiag[GetAntiDiag(sq)] & ~(GetSingleBitBb(sq) - 1);
        if (bb)
            SetBit(attack_set, GetLsb(bb));

        return attack_set;
    }

    /// Generates all the attack sets of a vector of occupancies.
    ///
    /// @param occupancies All occupancies for which we need to generate the attack sets.
    /// @param sq          Position of the attacking piece.
    /// @return Vector of bitboard representing the attack sets.
    std::vector<Bb> GenerateAttackSets(std::vector<Bb> occupancies, Sq sq)
    {
        std::vector<Bb> attack_sets;
        for each (Bb occupancy in occupancies)
        {
            attack_sets.push_back(GenerateAttackSet(occupancy, sq));
        }

        return attack_sets;
    }

    /// Generate a random number containing only a few bits set.
    ///
    /// @param rng Random number generator.
    /// @return The random number generated.
    std::uint64_t GenerateRndFewBits(std::mt19937_64& rng)
    {
        std::uniform_int_distribution<std::uint64_t> uniform_distr;
        std::uint64_t random;

        do
        {
            random = uniform_distr(rng) & uniform_distr(rng) & uniform_distr(rng);
        } while (GetPopct(random) > 6);
        

        return random;
    }

    /// Calculate the magic index.
    ///
    /// @param occupancy Occupancy of the relevant positions.
    /// @param magic     Magic number.
    /// @param shift     Number of position to right shift.
    /// @return Magic index.
    Bb CalculateMagicIndex(Bb occupancy, Bb magic, std::uint32_t shift)
    {
        return (occupancy * magic) >> shift;
    }

    /// Find a magic number for a piece on a square by trial and error.
    ///
    /// @param sq       Square.
    /// @param is_rook  Indicate if the piece is a rook or bishop.
    /// @param shift    Returns the number of bits of the shift necessary to calculate 
    ///                 the magic index.
    /// @return Magic number.
    std::uint64_t FindMagic(Sq sq, bool is_rook, std::uint32_t& shift)
    {
        std::random_device rd;
        std::mt19937_64 rng(rd());

        m8::Bb mask = GenerateMask(sq, is_rook);
        std::uint32_t num_bits = static_cast<std::uint32_t>(GetPopct(mask));
        shift = 64 - num_bits;

        auto occupancies = GenerateOccupancyVariation(mask);
        auto attack_sets = GenerateAttackSets(occupancies, sq);

        bool fail;
        std::uint64_t magic;

        std::unique_ptr<std::array<Bb, 4096>> used(new std::array<Bb, 4096>());

        do
        {
            magic = GenerateRndFewBits(rng);
            fail = false;
            used->fill(BB_C(0));

            std::uint32_t index_occup = 0;
            while (!fail && index_occup < occupancies.size())
            {
                Bb magic_index = CalculateMagicIndex(occupancies[index_occup], magic, shift);
                if (!(*used)[magic_index])
                {
                    (*used)[magic_index] = attack_sets[index_occup];
                }
                else
                {
                    if ((*used)[magic_index] != attack_sets[index_occup])
                    {
                        fail = true;
                    }
                }

                ++index_occup;
            }
        } while (fail);

        return magic;
    }

    /// Generates all the magic numbers for a piece.
    ///
    /// @param is_rook Indicate if the piece is a rook or a bishop.
    /// @param magics  Returns the magic numbers.
    /// @param shifts  Returns the shifts associated with the magic numbers.
    void GenerateMagics(bool is_rook, std::vector<std::uint64_t>& magics, std::vector<std::uint8_t>& shifts)
    {
        for (m8::Sq sq = m8::kA1; m8::IsSqOnBoard(sq); ++sq)
        {
            std::uint64_t magic;
            std::uint32_t shift;
            magic = m8::FindMagic(sq, is_rook, shift);

            magics.push_back(magic);
            shifts.push_back(shift);
        }
    }

    /// Output magic numbers.
    ///
    /// @param out           Stream to output the magic numbers to.
    /// @param variable_name Name of the magic number array.
    /// @param magics        Magic numbers
    void OutputMagics(std::ostream& out, const std::string& variable_name, const std::vector<std::uint64_t>& magics)
    {
        // Saves all current flag and format on the output stream
        boost::io::ios_all_saver guard(out);

        out << std::setfill('0') <<std::hex;

        out << "const std::array<const Bb, 64> " << variable_name << " = \n"
            << "{\n";

        for (auto x = 0; x < 64; ++x)
        {
            out << "    BB_C(0x" << std::setw(16) << magics[x] << ')';

            if (x < 63)
                out << ',';

            out << '\n';
        }

        out << "};\n" << std::endl;
    }

    /// Output the shifts values
    ///
    /// @param out           Stream to output the shift values to.
    /// @param variable_name Name of the shift values array.
    /// @param magics        Shift values
    void OutputShifts(std::ostream& out, const std::string& variable_name, const std::vector<std::uint8_t>& shifts)
    {
        // Saves all current flag and format on the output stream
        boost::io::ios_all_saver guard(out);

        out << std::setfill(' ');

        out << "const std::array<const std::uint32_t, 64> " << variable_name << " = \n"
            << "{\n";

        for (auto line = 0; line < 8; ++line)
        {
            out << "    ";
            for (auto col = 0; col < 8; ++col)
            {
                out << std::setw(2) << static_cast<std::uint32_t>(shifts[line * 8 + col]);
                if (line < 7 || col < 7)
                {
                    out << ',';
                    if (col < 7)
                    {
                        out << ' ';
                    }
                }
            }

            out << '\n';
        }

        out << "};\n" << std::endl;
    }
}


/// Main function. The entry point of the application.
///
/// @param argc Number of arguments in argv
/// @param argv Arguments passed on the command line when the application is started.
/// @returns Return code of the application. Should be 0 if the application terminated 
///          correctly and another value otherwise.
int main(int argc, char* argv[])
{
    std::vector<std::uint64_t> magics;
    std::vector<std::uint8_t> shifts;
    
    m8::GenerateMagics(true, magics, shifts);
    m8::OutputMagics(std::cout, "kRookMagics", magics);
    m8::OutputShifts(std::cout, "kRookMagicShifts", shifts);

    magics.clear();
    shifts.clear();

    m8::GenerateMagics(false, magics, shifts);
    m8::OutputMagics(std::cout, "kBishopMagics", magics);
    m8::OutputShifts(std::cout, "kBishopMagicShifts", shifts);

    return 0;
}