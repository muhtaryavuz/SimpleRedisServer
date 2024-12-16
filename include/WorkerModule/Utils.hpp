#pragma once

#include <atomic>

template <typename T>
struct AtomicWrapper {
  std::atomic<T> m_var;

  AtomicWrapper() : m_var() {}

  AtomicWrapper(const std::atomic<T>& a) : m_var(a.load()) {}

  AtomicWrapper(const AtomicWrapper& other) : m_var(other.m_var.load()) {}

  AtomicWrapper& operator=(const AtomicWrapper& other) {
    m_var.store(other.m_var.load());
  }

  bool operator<(const AtomicWrapper& other) {
    return m_var.load() < other.m_var.load();
  }
};