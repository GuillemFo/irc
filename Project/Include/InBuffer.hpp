/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InBuffer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/05 00:01:14 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INBUFFER_HPP
# define INBUFFER_HPP

# include <string>
# include <stdexcept>

class InBuffer {
	private:
		std::string _buffer;
		static const size_t _maxSize = 8192;

	public:
		InBuffer();
		InBuffer(const InBuffer& src);
		InBuffer& operator=(const InBuffer& src);
		~InBuffer();

		void append(const std::string& data);
		bool hasCompleteCommand() const;
		std::string extractCommand();
		bool overflow() const;
		void clear();
		const std::string& getRaw() const;
};

#endif
