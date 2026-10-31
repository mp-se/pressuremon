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
import js from '@eslint/js'
import pluginVue from 'eslint-plugin-vue'
import globals from 'globals'
import skipFormatting from '@vue/eslint-config-prettier/skip-formatting'

export default [
  {
    name: 'app/files-to-lint',
    files: ['**/*.{js,mjs,jsx,vue}']
  },

  {
    name: 'app/files-to-ignore',
    ignores: ['**/dist/**', '**/dist-ssr/**', '**/coverage/**']
  },

  {
    languageOptions: {
      globals: {
        ...globals.browser
      }
    }
  },

  {
    name: 'app/test-files',
    files: ['src/**/*.{test,spec}.js', 'src/tests/**/*.js'],
    languageOptions: {
      globals: {
        ...globals.browser,
        ...globals.node,
        ...globals.vitest
      }
    }
  },

  js.configs.recommended,
  ...pluginVue.configs['flat/essential'],
  skipFormatting
]
