#ifndef VALIDATION_HPP
# define VALIDATION_HPP

# include <vector>
# include "Request.hpp"


int		validateRequest(Request const & request, std::vector<std::string> & auth);

#endif