import { describe, it, expect } from 'vitest'
import fs from 'node:fs'
import path from 'node:path'
import en from '../en.json'

const SRC_DIR = path.resolve(__dirname, '../../')

function collectSourceFiles(dir) {
  const files = []
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    if (entry.name === '__tests__' || entry.name === 'locales') continue
    const full = path.join(dir, entry.name)
    if (entry.isDirectory()) {
      files.push(...collectSourceFiles(full))
    } else if (entry.name.endsWith('.vue') || entry.name.endsWith('.js')) {
      files.push(full)
    }
  }
  return files
}

function leafKeys(obj, prefix = '') {
  const keys = []
  for (const k of Object.keys(obj)) {
    const full = prefix ? `${prefix}.${k}` : k
    if (obj[k] !== null && typeof obj[k] === 'object') {
      keys.push(...leafKeys(obj[k], full))
    } else {
      keys.push(full)
    }
  }
  return keys
}

// Matched per-file (not on one joined blob) so a t(...) call at the end of one
// file can't span into an unrelated quoted string at the start of the next.
const staticCallKeys = new Set()
const dynamicNamespaces = new Set()
const runtimeNamespaces = new Set()

for (const file of collectSourceFiles(SRC_DIR)) {
  const text = fs.readFileSync(file, 'utf-8')

  // Matches static calls like t('namespace.key') / $t("namespace.key").
  // Negative lookbehind avoids matching mid-identifier, e.g. createWebSocket(.
  // Negative lookahead skips dynamic concatenation, e.g. t('support.' + key).
  for (const m of text.matchAll(/(?<![\w$])\$?t\(\s*['"]([\w.]+)['"](?!\s*\+)/g)) {
    staticCallKeys.add(m[1])
  }

  // Matches dynamic namespace concatenation like t('support.' + key)
  for (const m of text.matchAll(/(?<![\w$])\$?t\(\s*['"]([\w]+)\.['"]\s*\+/g)) {
    dynamicNamespaces.add(m[1])
  }

  // Matches template-literal lookups like t(`messages.${code}`) / te(`messages.${code}`).
  // The key itself is only known at runtime (e.g. a firmware-supplied message code), so
  // every key under such a namespace is treated as used rather than matched by name.
  for (const m of text.matchAll(/(?<![\w$])\$?te?\(\s*`([\w]+)\.\$\{/g)) {
    runtimeNamespaces.add(m[1])
  }
}

const sourceText = collectSourceFiles(SRC_DIR)
  .map((f) => fs.readFileSync(f, 'utf-8'))
  .join('\n')

function isKeyUsed(fullKey) {
  if (staticCallKeys.has(fullKey)) return true

  const [namespace, ...rest] = fullKey.split('.')
  if (runtimeNamespaces.has(namespace)) return true
  const leaf = rest.join('.')
  if (dynamicNamespaces.has(namespace)) {
    const quoted = new RegExp(`['"]${leaf}['"]`)
    if (quoted.test(sourceText)) return true
  }
  return false
}

describe('locale consistency (en.json)', () => {
  const keys = leafKeys(en)

  it('has no keys that are unused anywhere in src/', () => {
    const unused = keys.filter((k) => !isKeyUsed(k))
    expect(unused).toEqual([])
  })

  it('has no t()/$t() calls referencing a key missing from en.json', () => {
    const keySet = new Set(keys)
    const missing = [...staticCallKeys].filter((k) => !keySet.has(k))
    expect(missing).toEqual([])
  })
})
