#pragma once
#include <algorithm>
#include <format>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// > 754.txt && type 754.txt
namespace vert_calc { // 754方法训练终极目标：7分钟内5道4位数乘法全对
namespace details {
template<std::ranges::input_range _Rng, typename _Func, typename... _Args>
    requires std::invocable<_Func, _Args..., std::ranges::range_reference_t<_Rng>>
auto
RngForEach(_Rng&& range, _Func&& func, _Args&&... args) {
    return std::ranges::for_each(range, [&](auto&& item) {
        return std::invoke(std::forward<_Func>(func), std::forward<_Args>(args)..., item); });
}

inline static struct _Calc_Context {
    static constexpr std::string_view title{ "754方法训练终极目标：7分钟内5道4位数乘法全对" };
    static constexpr struct _Page {                         // 记事本(打印首先项, 方向: 横向)
        static constexpr std::size_t defaultSize{ 2 };      //   字体: 新宋体, 字形: 常规, 大小: 小四
        static constexpr std::size_t maxSize{ 20 };         //   页: 41行, 111列

        static constexpr char borderV{ '|' };
        static constexpr char borderH{ '_' };

        static constexpr std::size_t minFactor{ 2001 };
        static constexpr std::size_t maxFactor{ 9999 };
    } Page{};

} gCalcContext{};

namespace random {
std::size_t
rand() {
    using Distribution = std::uniform_int_distribution<std::size_t>;

    std::random_device r{};
    std::default_random_engine engine(r());
    Distribution uniform_dist(gCalcContext.Page.minFactor, gCalcContext.Page.maxFactor);
    return uniform_dist(engine);
}
} // end random namespace

struct _Calc_Product {
    std::size_t xFactor{};
    std::size_t yFactor{};
    std::size_t xyProduct{};

    auto operator()() {
        xFactor = random::rand();
        yFactor = random::rand();
        return xyProduct = xFactor * yFactor; }
};
using Products = std::vector<_Calc_Product>;

template <typename... Args>
std::string dyna_vformat(std::string_view rt_fmt_str, Args&&... args) {
    return std::vformat(rt_fmt_str, std::make_format_args(args...));
}

struct _Calc_Page {
private:
    static constexpr std::size_t productCount{ 10 };
    static constexpr std::size_t cellSize{ 21 };
    static constexpr std::size_t blankRowCount{ 13 };
    static std::string borderLine(bool isBlank = false, bool withSeparation = true) {
        auto fnEnd = [](bool withSeparation, std::size_t idx) -> char {
            bool isEnd = ((idx + 1) >= _Calc_Page::productCount / 2);
            if (!isEnd && !withSeparation)
                return gCalcContext.Page.borderH;

            return gCalcContext.Page.borderV;
        };

        std::stringstream ss;
        ss << gCalcContext.Page.borderV;
        for (std::size_t idx = 0; idx < _Calc_Page::productCount / 2; ++idx)
            ss << std::string(_Calc_Page::cellSize
                , (isBlank ? ' ' : gCalcContext.Page.borderH))
            << fnEnd(withSeparation, idx);
        return ss.str();
    }

    static const std::string& cellFormat() {
        auto fnCallOnce = [] {
            std::stringstream ss;
            ss << "{:<" << _Calc_Page::cellSize << "}"
                << gCalcContext.Page.borderV;
            return ss; };

        static const auto fmt = fnCallOnce().str();
        return fmt;
    }

    static std::vector<Products> getTwoRows(const Products& products) {
        const auto begin = products.cbegin();
        const auto mid = begin + products.size() / 2;
        const auto end = products.cend();
        return { Products{ begin, mid }, Products{ mid, end } };
    }

    static std::string getFirst(const Products& products, std::size_t idx) {
        static const auto fmt = cellFormat();
        return dyna_vformat(fmt
            , std::format("{}) {} × {}", (idx + 1), products[idx].xFactor, products[idx].yFactor));
    }
    static std::string getSecond(const Products& products, std::size_t idx
        , bool withResult = false
    ) {
        static const auto fmt = cellFormat();
        return dyna_vformat(fmt
            , std::format("  = {}", withResult ? std::to_string(products[idx].xyProduct) : ""));
    }

    static void coutBlanks(std::size_t rows) {
        for (std::size_t idx = 0; idx < rows; ++idx)
            std::cout << _Calc_Page::borderLine(true) << std::endl;
    }
    static void coutHead(std::size_t pageNo, bool tagResult = false) {
        std::cout                                                               // Line: 1, 2
            << "Ryan " << gCalcContext.title
            << (tagResult ? "答案" : "")
            << "(Page: " << pageNo << ")" << std::endl
            << _Calc_Page::borderLine(false, false) << std::endl;
    }
    static void coutTail() {
        auto fnCallOnce = [] {
            static constexpr auto fullSize
                = ((_Calc_Page::productCount / 2) * (_Calc_Page::cellSize + 1) - 1);

            std::stringstream ss;
            ss << gCalcContext.Page.borderV
                << "{:>" << fullSize << "}"
                << gCalcContext.Page.borderV;

            return dyna_vformat(ss.str()
                , std::format("{}", "Date:           Minutes:        ")); };

        static const std::string dateTag = fnCallOnce();
        std::cout << dateTag << std::endl;                                      // Line: 20
        std::cout << _Calc_Page::borderLine(false, false) << std::endl;         // Line: 21
    }

    static void coutRowProducts(const Products& products, bool withResult = false) {
        auto fnCallOnce = [] {
            static std::string_view fmt_5{ R"({}{}{}{}{})" };
            std::stringstream ss;
            ss << gCalcContext.Page.borderV << fmt_5
                << std::endl << gCalcContext.Page.borderV << fmt_5;

            return ss.str(); };

        auto fnWrapGetSecond = [withResult](auto&& _1, auto&& _2) {
            return getSecond(_1, _2, withResult); };

        static const auto fmt = fnCallOnce();
        std::cout << dyna_vformat(fmt                                           // Line: 3, 4
            , getFirst(products, 0)
            , getFirst(products, 1)
            , getFirst(products, 2)
            , getFirst(products, 3)
            , getFirst(products, 4)
            , fnWrapGetSecond(products, 0)
            , fnWrapGetSecond(products, 1)
            , fnWrapGetSecond(products, 2)
            , fnWrapGetSecond(products, 3)
            , fnWrapGetSecond(products, 4))
            << std::endl;
    }
    static void coutProducts(const Products& products) {
        coutRowProducts(products);                                              // Line: 3, 4
        std::cout << _Calc_Page::borderLine() << std::endl;                     // Line: 5
        coutBlanks(_Calc_Page::blankRowCount);                                  // Line: 6~18
        std::cout << _Calc_Page::borderLine() << std::endl;                     // Line: 19
        coutTail();                                                             // Line: 20, 21
    }

    static void coutResultProducts(const Products& products) {
        coutRowProducts(products, true);                                        // Line: 3, 4
        std::cout << _Calc_Page::borderLine() << std::endl;                     // Line: 5
    }

    Products products{};

public:
    void operator()() {
        products.clear();
        products.resize(_Calc_Page::productCount);
        RngForEach(products, [](auto&& product) { product(); }); };

    void Make(std::size_t pageNo) const {
        coutHead(pageNo);                                                       // Line: 1, 2
        RngForEach(getTwoRows(products), _Calc_Page::coutProducts);
        endl(std::cout);                                                        // Line: 41
    }

    void MakeResult(std::size_t pageNo) const {
        coutHead(pageNo, true);                                                 // Line: 1, 2
        RngForEach(getTwoRows(products), _Calc_Page::coutResultProducts);
        if (!(pageNo % (_Calc_Page::productCount / 2)))
            endl(std::cout);
    }
};
using Pages = std::vector<_Calc_Page>;

namespace homework {
Pages
pages(std::size_t page
) {
    Pages pages{ page };
    RngForEach(pages, [](auto&& page) { page(); });

    return pages;
}
} // end homework namespace

void
make(std::size_t page) {
    const auto pages = homework::pages(page);
    RngForEach(pages, [pageNo = 1](auto&& page) mutable { page.Make(pageNo++); });
    RngForEach(pages, [pageNo = 1](auto&& page) mutable { page.MakeResult(pageNo++); });
}

void
Make(std::size_t page) {
    if (page > gCalcContext.Page.maxSize)
        page = gCalcContext.Page.defaultSize;

    make(page);
}

void
Make() {
    Make(gCalcContext.Page.maxSize);
}
} // end details namespace

void Make() {
    details::Make();
}
} // end vert_calc namespace
