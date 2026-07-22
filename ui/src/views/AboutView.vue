<!--
  Pressuremon - Dual License

  Open Source Use  ->  GNU General Public License v3.0 (GPL v3)
  Commercial Use   ->  Commercial License Agreement

  Copyright (c) 2024-2026 Magnus

  For full license terms and conditions, see LICENSE and LICENSE_COMMERCIAL files.
  Source: https://github.com/mp-se/pressuremon and https://github.com/mp-se/pressuremon-ui
-->
<template>
  <div class="container">
    <p></p>
    <p class="h3">{{ t('about.title') }}</p>
    <hr />

    <p class="fw-normal">
      {{ t('about.intro') }}
    </p>

    <p class="h4">{{ t('about.license_title') }}</p>

    <p class="fw-normal">{{ t('about.license_intro') }}</p>

    <ul>
      <li>
        <strong>{{ t('about.license_opensource_label') }}:</strong>
        {{ t('about.license_opensource_text') }}
      </li>
      <li>
        <strong>{{ t('about.license_commercial_label') }}:</strong>
        {{ t('about.license_commercial_text') }}
      </li>
    </ul>

    <p class="fw-normal">
      {{ t('about.copyright') }}
      <a href="https://github.com/mp-se/pressuremon" target="_blank">
        https://github.com/mp-se/pressuremon
      </a>
      and
      <a href="https://github.com/mp-se/pressuremon-ui" target="_blank">
        https://github.com/mp-se/pressuremon-ui
      </a>
    </p>
  </div>
</template>

<script setup>
import { useI18n } from 'vue-i18n'

const { t } = useI18n()
</script>
