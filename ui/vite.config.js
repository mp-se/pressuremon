/*
 * Pressuremon - Dual License
 *
 * Open Source Use  ->  GNU General Public License v3.0 (GPL v3)
 * Commercial Use   ->  Commercial License Agreement
 *
 * Copyright (c) 2024-2026 Magnus
 *
 * For full license terms and conditions, see LICENSE and LICENSE_COMMERCIAL files.
 * Source: https://github.com/mp-se/pressuremon and https://github.com/mp-se/pressuremon-ui
 */
import { fileURLToPath, URL } from 'node:url'
import viteCompression from 'vite-plugin-compression'
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    vue({
      template: {
        compilerOptions: {
          // Remove comments and whitespace in production
          hoistStatic: true,
          cacheHandlers: true
        }
      }
    }),
    viteCompression({
      algorithm: 'gzip',
      threshold: 1024,
      deleteOriginFile: false
    })
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  },
  server: {
    proxy: {
      '/api': {
        target: 'http://localhost:3000',
        changeOrigin: true,
        secure: false
      }
    }
  },
  define: {
    __VUE_OPTIONS_API__: false, // Disable Options API if not used
    __VUE_PROD_DEVTOOLS__: false,
    // Ensure Pinia is included in the build
    'process.env.NODE_ENV': JSON.stringify('production')
  },
  optimizeDeps: {
    include: ['pinia', 'vue'], // Ensure Pinia and Vue are pre-bundled and optimized
    force: true // Force re-optimization to ensure Pinia is properly included
  },
  esbuild: {
    // Force consistent handling of Pinia modules
    platform: 'browser',
    format: 'esm'
  },
  css: {
    preprocessorOptions: {
      scss: {
        api: 'modern-compiler', // Use modern Sass API
        silenceDeprecations: ['legacy-js-api', 'import', 'global-builtin', 'color-functions'],
        quietDeps: true // Suppress warnings from dependencies
      }
    }
  },

  build: {
    minify: 'terser',
    cssCodeSplit: false,
    sourcemap: false,
    target: 'es2015',
    chunkSizeWarningLimit: 1000, // Disable chunk size warning (default is 500kB)
    terserOptions: {
      compress: {
        drop_console: false, // Keep console for debugging
        drop_debugger: true,
        passes: 2,
        unsafe: false, // Disable unsafe optimizations that might break code
        unsafe_comps: false,
        unsafe_Function: false,
        unsafe_math: false,
        unsafe_methods: false,
        unsafe_proto: false,
        unsafe_regexp: false,
        unsafe_undefined: false,
        side_effects: false
      },
      mangle: {
        properties: false // Disable property mangling to avoid breaking Vue
      },
      format: {
        comments: false
      }
    },
    rollupOptions: {
      output: {
        entryFileNames: `assets/[name].js`,
        chunkFileNames: `assets/[name].js`,
        assetFileNames: `assets/[name].[ext]`,
        manualChunks: undefined
      }
    }
  }
})
