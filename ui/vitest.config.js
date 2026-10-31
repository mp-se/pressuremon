import { fileURLToPath, URL } from 'node:url'
import { defineConfig } from 'vitest/config'
import vue from '@vitejs/plugin-vue'

export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  },
  test: {
    globals: true,
    environment: 'jsdom',
    setupFiles: 'src/tests/setup.js',
    coverage: {
      provider: 'istanbul',
      include: ['src/**/*.{js,vue}'],
      exclude: ['src/tests/**', 'src/**/__tests__/**', 'src/main.js'],
      reporter: ['text', 'lcov', 'html'],
      reportsDirectory: './coverage'
    }
  }
})