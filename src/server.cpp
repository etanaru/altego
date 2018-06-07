/**
 * server.cpp
 *
 * MIT License
 *
 * Copyright (c) 2018 LandZERO
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "server.h"

altego::Server::Server() : dlib::server_iostream() { set_graceful_close_timeout(1000); }

void altego::Server::SetResultStore(altego::ResultStore *resultStore) { _resultStore = resultStore; }

void altego::Server::on_connect(std::istream &in, std::ostream &out, const std::string &foreign_ip, const std::string &local_ip, unsigned short foreign_port,
                                unsigned short local_port, dlib::uint64 connection_id) {
  (void)in;
  (void)foreign_ip;
  (void)local_ip;
  (void)foreign_port;
  (void)local_port;
  std::cout << "server: new connection [" << connection_id << "]" << std::endl;
  while (_resultStore != nullptr && out.good()) {
    _resultStore->Next().Serialize(out);
  }
  std::cout << "server: connection [" << connection_id << "] closed" << std::endl;
}
