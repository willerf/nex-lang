#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_case_info.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <iostream>
#include <map>
#include <set>

class CustomListener: public Catch::EventListenerBase {
    std::map<std::string, int> passed_tests_by_tag;

  public:
    using EventListenerBase::EventListenerBase;

    void testCaseEnded(Catch::TestCaseStats const& test_case_stats) override {
        if (test_case_stats.totals.assertions.allPassed()) {
            std::cout << "\033[32mTest passed:\033[0m "
                      << test_case_stats.testInfo->name
                      << " - Assertions: \033[34m"
                      << test_case_stats.totals.assertions.passed
                      << "\033[0m\n";

            for (const auto& tag : test_case_stats.testInfo->tags) {
                std::string tag_name = static_cast<std::string>(tag.original);
                passed_tests_by_tag[tag_name]++;
            }
        }
    }

    void testRunEnded(Catch::TestRunStats const&) override {
        std::cout << "\n\033[1;36mSummary by Tags:\033[0m\n";
        for (const auto& pair : passed_tests_by_tag) {
            std::cout << "\033[1;33mTag [" << pair.first << "]:\033[0m \033[34m"
                      << pair.second << "\033[0m test(s) passed\n";
        }
    }
};

CATCH_REGISTER_LISTENER(CustomListener)
