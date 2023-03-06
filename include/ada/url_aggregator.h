/**
 * @file url_aggregator.h
 * @brief Declaration for the basic URL definitions
 */
#ifndef ADA_URL_AGGREGATOR_H
#define ADA_URL_AGGREGATOR_H

#include "ada/common_defs.h"
#include "ada/url_base.h"
#include "ada/url_components.h"

#include <string>
#include <string_view>

namespace ada {

  struct url_aggregator: virtual url_base {

    std::string buffer{};

    url_components components;

    bool set_username(const std::string_view input);
    bool set_password(const std::string_view input);
    bool set_href(const std::string_view input);
    bool set_port(const std::string_view input);
    void set_search(const std::string_view input);
    bool set_pathname(const std::string_view input);
    bool set_host(const std::string_view input);
    bool set_hostname(const std::string_view input);
    bool set_protocol(const std::string_view input);

    [[nodiscard]] ada_really_inline ada::url_components get_components() noexcept;

    [[nodiscard]] std::string get_origin() const noexcept;
    [[nodiscard]] std::string get_href() const noexcept;
    [[nodiscard]] std::string get_username() const noexcept;
    [[nodiscard]] std::string get_password() const noexcept;
    [[nodiscard]] std::string get_port() const noexcept;
    [[nodiscard]] std::string get_hash() const noexcept;
    [[nodiscard]] std::string get_host() const noexcept;
    [[nodiscard]] std::string get_hostname() const noexcept;
    [[nodiscard]] std::string get_pathname() const noexcept;
    [[nodiscard]] std::string get_search() const noexcept;
    [[nodiscard]] std::string get_protocol() const noexcept;

    /** @private */
    void update_base_fragment(const std::string_view input);
    /** @private */
    void update_base_search(std::optional<std::string> input);
    /** @private */
    void update_base_pathname(const std::string_view input);
    /** @private */
    bool base_fragment_has_value() const;
    /** @private */
    bool base_search_has_value() const;
    /** @private */
    bool base_port_has_value() const;

  };

} // namespace ada

#endif