#include <gtest/gtest.h>
#include <complex>

#include "core/plane_field.hpp"

namespace {
constexpr auto kDefaultSize = 4;
constexpr std::complex<double> kDefaultRawData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
}  // namespace

TEST(PlaneFieldTest, Construction) {
    EXPECT_NO_THROW({ diffraction::PlaneField field1(kDefaultSize, kDefaultSize); });

    EXPECT_NO_THROW({ diffraction::PlaneField field2(kDefaultSize, kDefaultSize, kDefaultRawData); });
}

TEST(PlaneFieldTest, Iteration) {
    diffraction::PlaneField field(kDefaultSize, kDefaultSize, kDefaultRawData);

    std::size_t acc = 1;

    for (auto& value : field) {
        EXPECT_EQ(value.real(), acc++);
    }

    acc = 1;

    for (auto y = 0; y < field.getYSize(); ++y) {
        for (auto x = 0; x < field.getXSize(); ++x) {
            EXPECT_EQ(field[y][x].real(), acc++);
        }
    }

    acc = 16;
    for (auto it = field.rbegin(), end = field.rend(); it != end; it++) {
        EXPECT_EQ(it->real(), acc--);
    }
}

TEST(PlaneFieldTest, Iterators) {
    diffraction::PlaneField field(kDefaultSize, kDefaultSize, kDefaultRawData);

    auto it = field.begin();

    EXPECT_EQ(it->real(), 1);

    EXPECT_EQ((*it++).real(), 1);
    EXPECT_EQ((*it).real(), 2);
    EXPECT_EQ((++it)->real(), 3);

    EXPECT_EQ((*it--).real(), 3);
    EXPECT_EQ((--it)->real(), 1);

    EXPECT_EQ(it[0].real(), 1);
    EXPECT_EQ(it[5].real(), 6);

    EXPECT_EQ((it + 3)->real(), 4);

    it += 3;

    EXPECT_EQ((it - 1)->real(), 3);

    it -= 3;

    EXPECT_EQ(it->real(), 1);

    EXPECT_EQ(it, field.begin());
    EXPECT_NE(it, field.end());

    *it = 0;
    EXPECT_EQ(it->real(), 0);

    auto rIt = field.rbegin();

    EXPECT_EQ(rIt->real(), 16);
    EXPECT_EQ((++rIt)->real(), 15);

    *rIt = 0;
    EXPECT_EQ(rIt->real(), 0);
}
