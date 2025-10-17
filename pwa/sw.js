// This service worker provides basic offline capabilities for the PWA.
// It caches the main app shell files so the app can load even without an internet connection.

const CACHE_NAME = 'sos-control-cache-v1';
const urlsToCache = [
  'index.html',
  'manifest.json'
  // You can add paths to icons or other assets here
];

// Install event: open a cache and add the app shell files to it
self.addEventListener('install', event => {
  event.waitUntil(
    caches.open(CACHE_NAME)
      .then(cache => {
        console.log('Opened cache');
        return cache.addAll(urlsToCache);
      })
  );
});

// Fetch event: serve assets from cache if available, otherwise fetch from network
self.addEventListener('fetch', event => {
  event.respondWith(
    caches.match(event.request)
      .then(response => {
        // Cache hit - return response
        if (response) {
          return response;
        }
        // Not in cache - fetch from network
        return fetch(event.request);
      })
  );
});
