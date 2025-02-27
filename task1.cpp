#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
    if (nums1.size() > nums2.size()) {
        std::swap(nums1, nums2);
    }

    int m = nums1.size();
    int n = nums2.size();
    int left = 0, right = m;

    while (left <= right) {
        int mid1 = (left + right) / 2;
        int mid2 = (m + n + 1) / 2 - mid1;

        double ma1 = (mid1 == 0) ? std::numeric_limits<double>::lowest() : nums1[mid1 - 1];
        double ma2 = (mid2 == 0) ? std::numeric_limits<double>::lowest() : nums2[mid2 - 1];
        double mi1 = (mid1 == m) ? std::numeric_limits<double>::max() : nums1[mid1];
        double mi2 = (mid2 == n) ? std::numeric_limits<double>::max() : nums2[mid2];

        if (ma1 <= mi2 && ma2 <= mi1) {
            if ((m + n) % 2 == 0) {
                return (std::max(ma1, ma2) + std::min(mi1, mi2)) / 2.0;
            } else {
                return std::max(ma1, ma2);
            }
        } else if (ma1 > mi2) {
            right = mid1 - 1;
        } else {
            left = mid1 + 1;
        }
    }

    throw std::invalid_argument("Не удалось найти медиану");
}

int main() {
    std::vector<int> nums1, nums2;
    int n;

    std::cout << "Введите количество элементов первого массива: ";
    std::cin >> n;
    std::cout << "Введите элементы первого массива (через пробел): ";
    for (int i = 0; i < n; ++i) {
        int num;
        std::cin >> num;
        nums1.push_back(num);
    }

    std::cout << "Введите количество элементов второго массива: ";
    std::cin >> n;
    std::cout << "Введите элементы второго массива (через пробел): ";
    for (int i = 0; i < n; ++i) {
        int num;
        std::cin >> num;
        nums2.push_back(num);
    }

    try {
        double median = findMedianSortedArrays(nums1, nums2);
        std::cout << "Медиана: " << median << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

